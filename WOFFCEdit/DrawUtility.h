#pragma once
// Source: https://github.com/microsoft/DirectXTK/wiki/DebugDraw

#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

class DisplayObject;
struct SelectedObject;

namespace DrawUtility
{

	void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		const DirectX::BoundingSphere& sphere,
		DirectX::FXMVECTOR color = DirectX::Colors::White);
	
	void XM_CALLCONV Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		const DirectX::BoundingFrustum& frustum,
		DirectX::FXMVECTOR color = DirectX::Colors::White);

	
	void XM_CALLCONV DrawRing(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR origin, DirectX::FXMVECTOR majorAxis, DirectX::FXMVECTOR minorAxis,
		DirectX::GXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV DrawRay(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR origin, DirectX::FXMVECTOR direction, bool normalize = true,
		DirectX::FXMVECTOR color = DirectX::Colors::White);

	void XM_CALLCONV DrawLine(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR A, DirectX::FXMVECTOR B, DirectX::FXMVECTOR color);

	void XM_CALLCONV DrawTriangle(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch,
		DirectX::FXMVECTOR pointA, DirectX::FXMVECTOR pointB, DirectX::FXMVECTOR pointC,
		DirectX::GXMVECTOR color = DirectX::Colors::White);


}