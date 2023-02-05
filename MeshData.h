#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <map>
#include "Vertex.h"
#include "Lightings.h"

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
	std::vector<VertexPositionNormalTextureColor> vertices = {};
	// インデックス配列
	std::vector<UINT> indices = {};
	// マテリアルの名前
	std::string materialName = {};
	// マテリアル
	Material material = {};

	// テクスチャーの名前
	std::string textureName = {};
	// テクスチャー
	ComPtr<ID3D11ShaderResourceView> texture = nullptr;
};
