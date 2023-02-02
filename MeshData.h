#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "Vertex.h"

// メッシュデータ
struct MeshData
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 頂点バッファー
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	// インデックスバッファー
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;

	// 頂点数
	UINT vertexCount = 0;
	// 頂点配列
	std::vector<VertexPosition> vertices = {};
	// インデックス配列
	std::vector<UINT> indices = {};
};
