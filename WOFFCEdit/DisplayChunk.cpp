#include <string>
#include "DisplayChunk.h"
#include "Game.h"
#include <cmath>

using namespace DirectX;
using namespace DirectX::SimpleMath;

DisplayChunk::DisplayChunk()
{
	//terrain size in meters. note that this is hard coded here, we COULD get it from the terrain chunk along with the other info from the tool if we want to be more flexible.
	m_terrainSize = 512;
	m_terrainHeightScale = 0.25;  //convert our 0-256 terrain to 64
	m_textureCoordStep = 1.0 / (TERRAINRESOLUTION-1);	//-1 becuase its split into chunks. not vertices.  we want tthe last one in each row to have tex coord 1
	m_terrainPositionScalingFactor = m_terrainSize / (TERRAINRESOLUTION-1);
}


DisplayChunk::~DisplayChunk()
{
}

void DisplayChunk::PopulateChunkData(ChunkObject * SceneChunk)
{
	m_name = SceneChunk->name;
	m_chunk_x_size_metres = SceneChunk->chunk_x_size_metres;
	m_chunk_y_size_metres = SceneChunk->chunk_y_size_metres;
	m_chunk_base_resolution = SceneChunk->chunk_base_resolution;
	m_heightmap_path = SceneChunk->heightmap_path;
	m_tex_diffuse_path = SceneChunk->tex_diffuse_path;
	m_tex_splat_alpha_path = SceneChunk->tex_splat_alpha_path;
	m_tex_splat_1_path = SceneChunk->tex_splat_1_path;
	m_tex_splat_2_path = SceneChunk->tex_splat_2_path;
	m_tex_splat_3_path = SceneChunk->tex_splat_3_path;
	m_tex_splat_4_path = SceneChunk->tex_splat_4_path;
	m_render_wireframe = SceneChunk->render_wireframe;
	m_render_normals = SceneChunk->render_normals;
	m_tex_diffuse_tiling = SceneChunk->tex_diffuse_tiling;
	m_tex_splat_1_tiling = SceneChunk->tex_splat_1_tiling;
	m_tex_splat_2_tiling = SceneChunk->tex_splat_2_tiling;
	m_tex_splat_3_tiling = SceneChunk->tex_splat_3_tiling;
	m_tex_splat_4_tiling = SceneChunk->tex_splat_4_tiling;
}

void DisplayChunk::RenderBatch(std::shared_ptr<DX::DeviceResources>  DevResources)
{
	auto context = DevResources->GetD3DDeviceContext();

	m_terrainEffect->Apply(context);
	context->IASetInputLayout(m_terrainInputLayout.Get());

	m_batch->Begin();
	for (size_t i = 0; i < TERRAINRESOLUTION-1; i++)	//looping through QUADS.  so we subtrack one from the terrain array or it will try to draw a quad starting with the last vertex in each row. Which wont work
	{
		for (size_t j = 0; j < TERRAINRESOLUTION-1; j++)//same as above
		{
			m_batch->DrawQuad(m_terrainGeometry[i][j], m_terrainGeometry[i][j+1], m_terrainGeometry[i+1][j+1], m_terrainGeometry[i+1][j]); //bottom left bottom right, top right top left.
		}
	}
	m_batch->End();
}

void DisplayChunk::InitialiseBatch()
{
	//build geometry for our terrain array
	//iterate through all the vertices of our required resolution terrain.
	int index = 0;

	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_terrainGeometry[i][j].position =			Vector3(j*m_terrainPositionScalingFactor-(0.5*m_terrainSize), (float)(m_heightMap[index])*m_terrainHeightScale, i*m_terrainPositionScalingFactor-(0.5*m_terrainSize));	//This will create a terrain going from -64->64.  rather than 0->128.  So the center of the terrain is on the origin
			m_terrainGeometry[i][j].normal =			Vector3(0.0f, 1.0f, 0.0f);						//standard y =up
			m_terrainGeometry[i][j].textureCoordinate =	Vector2(((float)m_textureCoordStep*j)*m_tex_diffuse_tiling, ((float)m_textureCoordStep*i)*m_tex_diffuse_tiling);				//Spread tex coords so that its distributed evenly across the terrain from 0-1
			
		}
	}
	CalculateTerrainNormals();

	//////////////////////////////////////////////////////////////////////////
	// Store original terrain data
	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (int j = 0; j < TERRAINRESOLUTION; j++)
		{
			m_terrainGeometryOriginal[i][j] = m_terrainGeometry[i][j];
		}
	}

	CalculateTerrainNormals();

	XMFLOAT3 A = m_terrainGeometry[0][0].position;
	XMFLOAT3 B = m_terrainGeometry[0][1].position;
	A.y = 0;
	B.y = 0;
	XMVECTOR length = XMVector3Length(B - A);
	m_RealRadius = length.m128_f32[0] * m_Radius;
}

void DisplayChunk::LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources)
{
	auto device = DevResources->GetD3DDevice();
	auto devicecontext = DevResources->GetD3DDeviceContext();

	//load in heightmap .raw
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.

	pFile = fopen(m_heightmap_path.c_str(), "rb");
	// Check To See If We Found The File And Could Open It
	if (pFile == NULL)
	{
		// Display Error Message And Stop The Function
		MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
		return;
	}

	// Here We Load The .RAW File Into Our pHeightMap Data Array
	// We Are Only Reading In '1', And The Size Is (Width * Height)
	fread(m_heightMap, 1, TERRAINRESOLUTION*TERRAINRESOLUTION, pFile);

	fclose(pFile);

	//load in texture diffuse
	
	//load the diffuse texture
	std::wstring texturewstr = StringToWCHART(m_tex_diffuse_path);
	HRESULT rs;	
	rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), NULL, &m_texture_diffuse);	//load tex into Shader resource	view and resource
	
	//setup terrain effect
	m_terrainEffect = std::make_unique<BasicEffect>(device);
	m_terrainEffect->EnableDefaultLighting();
	m_terrainEffect->SetLightingEnabled(true);
	m_terrainEffect->SetTextureEnabled(true);
	m_terrainEffect->SetTexture(m_texture_diffuse);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_terrainEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//setup batch
	DX::ThrowIfFailed(
		device->CreateInputLayout(VertexPositionNormalTexture::InputElements,
			VertexPositionNormalTexture::InputElementCount,
			shaderByteCode,
			byteCodeLength,
			m_terrainInputLayout.GetAddressOf())
		);

	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormalTexture>>(devicecontext);

	
}

void DisplayChunk::SaveHeightMap()
{
/*	for (size_t i = 0; i < TERRAINRESOLUTION*TERRAINRESOLUTION; i++)
	{
		m_heightMap[i] = 0;
	}*/

	//for (int i = 0; i < TERRAINRESOLUTION; i++)
	//{
	//	for (int j = 0; j < TERRAINRESOLUTION; j++)
	//	{
	//		// Reverse height normalization when saving out
	//		float height = m_terrainGeometry[i][j].position.y / m_terrainHeightScale;

	//		if (height != 0)
	//		{
	//			int x = 3;
	//		}
	//		m_heightMap[i * TERRAINRESOLUTION + j] = height;
	//	}
	//}

	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.
	pFile = fopen(m_heightmap_path.c_str(), "wb+");;
	// Check To See If We Found The File And Could Open It
	if (pFile == NULL)
	{
		// Display Error Message And Stop The Function
		MessageBox(NULL, L"Can't Find The Height Map!", L"Error", MB_OK);
		return;
	}

	fwrite(m_heightMap, 1, TERRAINRESOLUTION*TERRAINRESOLUTION, pFile);
	fclose(pFile);
	
}

void DisplayChunk::UpdateTerrain()
{
	//all this is doing is transferring the height from the heigtmap into the terrain geometry.
	int index;
	for (size_t i = 0; i < TERRAINRESOLUTION; i++)
	{
		for (size_t j = 0; j < TERRAINRESOLUTION; j++)
		{
			index = (TERRAINRESOLUTION * i) + j;
			m_terrainGeometry[i][j].position.y = (float)(m_heightMap[index])*m_terrainHeightScale;	
		}
	}
	CalculateTerrainNormals();

}

void DisplayChunk::GenerateHeightmap()
{
	//insert how YOU want to update the heigtmap here! :D
}


std::vector<TriangleData> DisplayChunk::GetIntersectingTriangles(DirectX::SimpleMath::Ray _pickingRay)
{
	std::vector<TriangleData> intersectingTriangles;

	std::pair<int, int> ij0;
	std::pair<int, int> ij1;
	std::pair<int, int> ij2;
	std::pair<int, int> ij3;

	for (int i = 0; i < TERRAINRESOLUTION - 2; i++)
	{
		for (int j = 0; j < TERRAINRESOLUTION - 2; j++)
		{
			ij0 = { i,     j };
			ij1 = { i,     j + 1 };
			ij2 = { i + 1, j };
			ij3 = { i + 1, j + 1 };

			auto v0 = TerrainVertexData(ij0.first, ij0.second, m_terrainGeometry[ij0.first][ij0.second].position);
			auto v1 = TerrainVertexData(ij1.first, ij1.second, m_terrainGeometry[ij1.first][ij1.second].position);
			auto v2 = TerrainVertexData(ij2.first, ij2.second, m_terrainGeometry[ij2.first][ij2.second].position);
			auto v3 = TerrainVertexData(ij3.first, ij3.second, m_terrainGeometry[ij3.first][ij3.second].position);

			/* Quad:
				v0-----v1
				| \    |
				|  \   |
				|   \  |
				|    \ |
				v2----v3
			*/

			float distance;

			if (_pickingRay.Intersects(v0.position, v3.position, v1.position, distance))
			{
				intersectingTriangles.emplace_back(TriangleData(v0, v3, v1));
				intersectingTriangles.back().distance = distance;
			}

			if (_pickingRay.Intersects(v0.position, v2.position, v3.position, distance))
			{
				intersectingTriangles.emplace_back(TriangleData(v0, v2, v3));
				intersectingTriangles.back().distance = distance;
			}
		}
	}

	return intersectingTriangles;
}

void DisplayChunk::EditTerrain(TerrainVertexData _selectedVertex, bool _elevate)
{
	constexpr float adjustmentSpeed = 10;
	const float radiusSquared = m_Radius * m_Radius;

	const float deltaHeight = adjustmentSpeed;
	const float targetHeight = _selectedVertex.position.y + deltaHeight;

	int iMin = std::ceil(std::max(_selectedVertex.i - m_Radius, 0.0f));
	int jMin = std::ceil(std::max(_selectedVertex.j - m_Radius, 0.0f));
	int iMax = std::floor(std::min(_selectedVertex.i + m_Radius, TERRAINRESOLUTION - 1.0f));
	int jMax = std::floor(std::min(_selectedVertex.j + m_Radius, TERRAINRESOLUTION - 1.0f));

	for (int i = iMin; i <= iMax; i++)
	{
		for (int j = jMin; j <= jMax; j++)
		{
			// Check if within circle
			float dx = std::abs(i - _selectedVertex.i);
			float dy = std::abs(j - _selectedVertex.j);
			float D = dx * dx + dy * dy;

			if (D < radiusSquared)
			{
				float dist = sqrt(i * i + j * j);
				float alpha = dist != 0 ? (m_Radius / dist) : 0;

				if (_elevate)
				{
					m_terrainGeometry[i][j].position.y += deltaHeight * alpha;
				}
				else
				{
					m_terrainGeometry[i][j].position.y = std::max(m_terrainGeometry[i][j].position.y - deltaHeight * alpha, 0.0f);
				}
			}
		}
	}

	CalculateTerrainNormals();

}


void DisplayChunk::CalculateTerrainNormals()
{
	int index1, index2, index3, index4;
	DirectX::SimpleMath::Vector3 upDownVector, leftRightVector, normalVector;



	for (int i = 0; i<(TERRAINRESOLUTION - 1); i++)
	{
		for (int j = 0; j<(TERRAINRESOLUTION - 1); j++)
		{
			upDownVector.x = (m_terrainGeometry[i + 1][j].position.x - m_terrainGeometry[i - 1][j].position.x);
			upDownVector.y = (m_terrainGeometry[i + 1][j].position.y - m_terrainGeometry[i - 1][j].position.y);
			upDownVector.z = (m_terrainGeometry[i + 1][j].position.z - m_terrainGeometry[i - 1][j].position.z);

			leftRightVector.x = (m_terrainGeometry[i][j - 1].position.x - m_terrainGeometry[i][j + 1].position.x);
			leftRightVector.y = (m_terrainGeometry[i][j - 1].position.y - m_terrainGeometry[i][j + 1].position.y);
			leftRightVector.z = (m_terrainGeometry[i][j - 1].position.z - m_terrainGeometry[i][j + 1].position.z);


			leftRightVector.Cross(upDownVector, normalVector);	//get cross product
			normalVector.Normalize();			//normalise it.

			m_terrainGeometry[i][j].normal = normalVector;	//set the normal for this point based on our result
		}
	}
}
