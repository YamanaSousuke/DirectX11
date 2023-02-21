#pragma once
#pragma warning(disable : 26451)
#pragma warning(disable : 26495)
#pragma warning(disable : 4099)

#include <fbxsdk.h>
#include <DirectXMath.h>
#include "Model.h"

// FBXファイルから読み込みを行う
class FbxMeshFile
{
public:
	FbxMeshFile() = default;
	~FbxMeshFile() = default;

	FbxMeshFile(const FbxMeshFile&) = delete;
	FbxMeshFile& operator=(const FbxMeshFile&) = delete;

	FbxMeshFile(FbxMeshFile&&) = default;
	FbxMeshFile& operator=(FbxMeshFile&&) = default;

	// 初期化
	void Init(ID3D11Device* device);
	// ファイルの読み込み
	Model Load(const std::string& filename, ID3D11DeviceContext* immediateContext);
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ファイルからメッシュの生成を行う
	bool GenerateMeshFromFile(const std::string& filename);
	// マテリアルデータの読み込み
	void LoadMaterial(Model::MeshData& meshData, FbxMesh* mesh);
	// メッシュデータの作成
	void CreateMesh(FbxMesh* mesh);
	// 頂点インデックスデータを読み込む
	void LoadIndices(Model::MeshData& meshData, FbxMesh* mesh);
	// 頂点データを読み込む
	void LoadVertices(Model::MeshData& meshData, FbxMesh* mesh);
	// マテリアル名の設定
	void SetMaterial(Model::MeshData& meshData, FbxMesh* mesh);
	// 頂点カラーデータを読み込む
	void LoadColors(Model::MeshData& meshData, FbxMesh* mesh);
	// 法線データを読み込む
	void LoadNormal(Model::MeshData& meshData, FbxMesh* mesh);
	// UV座標の読み込み
	void LoadUV(Model::MeshData& meshData, FbxMesh* mesh);
	// 頂点バッファー、インデックスバッファーの作成
	void CreateBuffer(ID3D11DeviceContext* immediateContext);

	// デバイス
	ComPtr<ID3D11Device> device = nullptr;
	// 最後の「/」または「\\」で区切られた文字列の前半部分
	std::string fileNameBeforeSplit;
	// 最後の「/」または「\\」で区切られた文字列の後半部分
	std::string fileNameAfterSpilt;
	// モデル
	Model model = {};
};
