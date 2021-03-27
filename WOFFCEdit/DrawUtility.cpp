#include "pch.h"
#include "DrawUtility.h"
#include <algorithm>
#include "DisplayObject.h"

using namespace DirectX;

namespace DrawUtility
{

	void XM_CALLCONV Draw(PrimitiveBatch<VertexPositionColor>* batch,
		const BoundingSphere& sphere,
		FXMVECTOR color)
	{
		XMVECTOR origin = XMLoadFloat3(&sphere.Center);

		const float radius = sphere.Radius;

		XMVECTOR xaxis = g_XMIdentityR0 * radius;
		XMVECTOR yaxis = g_XMIdentityR1 * radius;
		XMVECTOR zaxis = g_XMIdentityR2 * radius;

		DrawRing(batch, origin, xaxis, zaxis, color);
		DrawRing(batch, origin, xaxis, yaxis, color);
		DrawRing(batch, origin, yaxis, zaxis, color);
	}


	
	
	void XM_CALLCONV Draw(PrimitiveBatch<VertexPositionColor>* batch,
		const BoundingFrustum& frustum,
		FXMVECTOR color)
	{
		XMFLOAT3 corners[BoundingFrustum::CORNER_COUNT];
		frustum.GetCorners(corners);

		VertexPositionColor verts[24] = {};
		verts[0].position = corners[0];
		verts[1].position = corners[1];
		verts[2].position = corners[1];
		verts[3].position = corners[2];
		verts[4].position = corners[2];
		verts[5].position = corners[3];
		verts[6].position = corners[3];
		verts[7].position = corners[0];

		verts[8].position = corners[0];
		verts[9].position = corners[4];
		verts[10].position = corners[1];
		verts[11].position = corners[5];
		verts[12].position = corners[2];
		verts[13].position = corners[6];
		verts[14].position = corners[3];
		verts[15].position = corners[7];

		verts[16].position = corners[4];
		verts[17].position = corners[5];
		verts[18].position = corners[5];
		verts[19].position = corners[6];
		verts[20].position = corners[6];
		verts[21].position = corners[7];
		verts[22].position = corners[7];
		verts[23].position = corners[4];

		for (size_t j = 0; j < _countof(verts); ++j)
		{
			XMStoreFloat4(&verts[j].color, color);
		}

		batch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINELIST, verts, _countof(verts));
	}


	
	void XM_CALLCONV DrawRing(PrimitiveBatch<VertexPositionColor>* batch,
		FXMVECTOR origin,
		FXMVECTOR majorAxis,
		FXMVECTOR minorAxis,
		GXMVECTOR color)
	{
		static const size_t c_ringSegments = 32;

		VertexPositionColor verts[c_ringSegments + 1];

		FLOAT fAngleDelta = XM_2PI / float(c_ringSegments);
		// Instead of calling cos/sin for each segment we calculate
		// the sign of the angle delta and then incrementally calculate sin
		// and cosine from then on.
		XMVECTOR cosDelta = XMVectorReplicate(cosf(fAngleDelta));
		XMVECTOR sinDelta = XMVectorReplicate(sinf(fAngleDelta));
		XMVECTOR incrementalSin = XMVectorZero();
		static const XMVECTORF32 s_initialCos =
		{
			1.f, 1.f, 1.f, 1.f
		};
		XMVECTOR incrementalCos = s_initialCos.v;
		for (size_t i = 0; i < c_ringSegments; i++)
		{
			XMVECTOR pos = XMVectorMultiplyAdd(majorAxis, incrementalCos, origin);
			pos = XMVectorMultiplyAdd(minorAxis, incrementalSin, pos);
			XMStoreFloat3(&verts[i].position, pos);
			XMStoreFloat4(&verts[i].color, color);
			// Standard formula to rotate a vector.
			XMVECTOR newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
			XMVECTOR newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;
			incrementalCos = newCos;
			incrementalSin = newSin;
		}
		verts[c_ringSegments] = verts[0];

		batch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, c_ringSegments + 1);
	}


	void XM_CALLCONV DrawRay(PrimitiveBatch<VertexPositionColor>* batch,
		FXMVECTOR origin,
		FXMVECTOR direction,
		bool normalize,
		FXMVECTOR color)
	{
		VertexPositionColor verts[3];
		XMStoreFloat3(&verts[0].position, origin);

		XMVECTOR normDirection = XMVector3Normalize(direction);
		XMVECTOR rayDirection = (normalize) ? normDirection : direction;

		XMVECTOR perpVector = XMVector3Cross(normDirection, g_XMIdentityR1);

		if (XMVector3Equal(XMVector3LengthSq(perpVector), g_XMZero))
		{
			perpVector = XMVector3Cross(normDirection, g_XMIdentityR2);
		}
		perpVector = XMVector3Normalize(perpVector);

		XMStoreFloat3(&verts[1].position, XMVectorAdd(rayDirection, origin));
		perpVector = XMVectorScale(perpVector, 0.0625f);
		normDirection = XMVectorScale(normDirection, -0.25f);
		rayDirection = XMVectorAdd(perpVector, rayDirection);
		rayDirection = XMVectorAdd(normDirection, rayDirection);
		XMStoreFloat3(&verts[2].position, XMVectorAdd(rayDirection, origin));

		XMStoreFloat4(&verts[0].color, color);
		XMStoreFloat4(&verts[1].color, color);
		XMStoreFloat4(&verts[2].color, color);

		batch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, 2);
	}


	void XM_CALLCONV DrawLine(PrimitiveBatch<VertexPositionColor>* batch, FXMVECTOR A, FXMVECTOR B, FXMVECTOR color)
	{
		DrawRay(batch, A, B - A, false, color);
	}

	void XM_CALLCONV DrawTriangle(PrimitiveBatch<VertexPositionColor>* batch,
		FXMVECTOR pointA,
		FXMVECTOR pointB,
		FXMVECTOR pointC,
		GXMVECTOR color)
	{
		VertexPositionColor verts[4];
		XMStoreFloat3(&verts[0].position, pointA);
		XMStoreFloat3(&verts[1].position, pointB);
		XMStoreFloat3(&verts[2].position, pointC);
		XMStoreFloat3(&verts[3].position, pointA);

		XMStoreFloat4(&verts[0].color, color);
		XMStoreFloat4(&verts[1].color, color);
		XMStoreFloat4(&verts[2].color, color);
		XMStoreFloat4(&verts[3].color, color);

		batch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, 4);
	}



}