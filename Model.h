#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <map>
#include "Vertex.h"
#include "Lightings.h"

// モデル
class Model
{
public:
	// メッシュデータ
	struct MeshData
	{
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
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;
		// 頂点バッファー
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
		// インデックスバッファー
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	};

	// モデル名の設定
	void SetModelName(const std::string& name);
	// モデル名の取得
	const char* GetModelName() const;
	// メッシュデータを配列に追加
	void PushMeshData(const MeshData& meshData);
	// メッシュデータの取得
	std::vector<MeshData>& GetMeshData();
private:
	// モデルの名前
	std::string modelName;
	// メッシュデータの配列
	std::vector<MeshData> meshList = {};
};
