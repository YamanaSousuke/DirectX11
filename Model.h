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
	Model() = default;
	~Model() = default;
	
	// Model(const Model&) = delete;
	// Model& operator=(const Model&) = delete;

	Model(const Model&) = default;
	Model& operator=(const Model&) = default;
	
	Model(const Model&&) = delete;
	Model& operator=(const Model&&) = delete;

	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// メッシュデータ
	struct MeshData {
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
		// 頂点バッファー
		ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
		// インデックスバッファー
		ComPtr<ID3D11Buffer> indexBuffer = nullptr;
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
