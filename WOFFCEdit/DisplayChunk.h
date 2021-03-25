#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "ChunkObject.h"

//geometric resoltuion - note,  hard coded.
#define TERRAINRESOLUTION 128


struct TerrainVertexData
{
	TerrainVertexData(int _i = -1, int _j = -1, DirectX::XMFLOAT3 _v = DirectX::XMFLOAT3(0, 0, 0)) : i(_i), j(_j), position(_v) {}
	int i, j;
	DirectX::XMFLOAT3 position;
};

struct TriangleData
{
	TriangleData(TerrainVertexData _v0, TerrainVertexData _v1, TerrainVertexData _v2)
		: v0(_v0), v1(_v1), v2(_v2) {}
	TerrainVertexData v0;
	TerrainVertexData v1;
	TerrainVertexData v2;
	float distance;

	bool operator<(const TriangleData& other)
	{
		return distance < other.distance;
	}
};

class DisplayChunk
{
public:
	DisplayChunk();
	~DisplayChunk();
	void PopulateChunkData(ChunkObject * SceneChunk);
	void RenderBatch(std::shared_ptr<DX::DeviceResources>  DevResources);
	void InitialiseBatch();	//initial setup, base coordinates etc based on scale
	void LoadHeightMap(std::shared_ptr<DX::DeviceResources>  DevResources);
	void SaveHeightMap();			//saves the heigtmap back to file.
	void UpdateTerrain();			//updates the geometry based on the heigtmap
	void GenerateHeightmap();		//creates or alters the heightmap
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormalTexture>>  m_batch;
	std::unique_ptr<DirectX::BasicEffect>       m_terrainEffect;

	ID3D11ShaderResourceView *					m_texture_diffuse;				//diffuse texture
	Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_terrainInputLayout;

	std::vector<TriangleData> GetIntersectingTriangles(DirectX::SimpleMath::Ray _pickingRay);
	void EditTerrain(TerrainVertexData _selectedVertex, bool _elevate);
	float GetBrushRadius() { return m_RealRadius; }

	// Set _paint to false to erase. 
	void ColorPaintTerrain(TerrainVertexData _selectedVertex, DirectX::XMFLOAT4 _color, bool _paint = true);
	DirectX::XMFLOAT4 LerpColor(DirectX::XMFLOAT4 _A, DirectX::XMFLOAT4 _B, float _t);


private:
	
	DirectX::VertexPositionNormalTexture m_terrainGeometry[TERRAINRESOLUTION][TERRAINRESOLUTION];
	DirectX::VertexPositionNormalTexture m_terrainGeometryOriginal[TERRAINRESOLUTION][TERRAINRESOLUTION];

	BYTE m_heightMap[TERRAINRESOLUTION*TERRAINRESOLUTION];
	
	void CalculateTerrainNormals();

	float	m_terrainHeightScale;
	int		m_terrainSize;				//size of terrain in metres
	float	m_textureCoordStep;			//step in texture coordinates between each vertex row / column
	float   m_terrainPositionScalingFactor;	//factor we multiply the position by to convert it from its native resolution( 0- Terrain Resolution) to full scale size in metres dictated by m_Terrainsize
	
	std::string m_name;
	int m_chunk_x_size_metres;
	int m_chunk_y_size_metres;
	int m_chunk_base_resolution;
	std::string m_heightmap_path;
	std::string m_tex_diffuse_path;
	std::string m_tex_splat_alpha_path;
	std::string m_tex_splat_1_path;
	std::string m_tex_splat_2_path;
	std::string m_tex_splat_3_path;
	std::string m_tex_splat_4_path;
	bool m_render_wireframe;
	bool m_render_normals;
	int m_tex_diffuse_tiling;
	int m_tex_splat_1_tiling;
	int m_tex_splat_2_tiling;
	int m_tex_splat_3_tiling;
	int m_tex_splat_4_tiling;

	float m_Radius = 5;
	float m_RealRadius;

};

