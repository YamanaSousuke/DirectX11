#pragma once
#pragma warning(disable : 26451)
#pragma warning(disable : 26495)
#pragma warning(disable : 4099)

#include <fbxsdk.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <map>
#include <DirectXMath.h>

#include "VertexShader.h"
#include "MeshData.h"
#include "Lightings.h"

class FbxMeshFile
{
public:
	// 初期化
	void Init(ID3D11Device* device);
	// ファイルの読み込み
	FbxMeshFile* Load(const std::string& filename, ID3D11DeviceContext* immediateContext);
	// メッシュ数の取得
	size_t GetMeshCount() const;
	// メッシュデータの取得
	MeshData GetMeshData(int index) const;

	std::vector<MeshData> GetMeshData() const;
	// Fbxファイル名の取得
	std::string GetFbxFileName() const;
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ファイルからメッシュの生成を行う
	bool GenerateMeshFromFile(const std::string& filename);
	// マテリアルデータの読み込み
	void LoadMaterial(MeshData& meshData, FbxMesh* mesh);
	// メッシュデータの作成
	void CreateMesh(FbxMesh* mesh);
	// 頂点インデックスデータを読み込む
	void LoadIndices(MeshData& meshData, FbxMesh* mesh);
	// 頂点データを読み込む
	void LoadVertices(MeshData& meshData, FbxMesh* mesh);
	// マテリアル名の設定
	void SetMaterial(MeshData& meshData, FbxMesh* mesh);
	// 頂点カラーデータを読み込む
	void LoadColors(MeshData& meshData, FbxMesh* mesh);
	// 法線データを読み込む
	void LoadNormal(MeshData& meshData, FbxMesh* mesh);
	// UV座標の読み込み
	void LoadUV(MeshData& meshData, FbxMesh* mesh);

	// 頂点バッファーの作成
	bool CreateVertexBuffer(ID3D11DeviceContext* immediateContext);
	// インデックスバッファーの作成
	bool CreateIndexBuffer(ID3D11DeviceContext* immediateContext);

	// メッシュデータ
	std::vector<MeshData> meshList = {};
	// デバイス
	ComPtr<ID3D11Device> device = nullptr;
	// 最後の「/」または「\\」で区切られた文字列の前半部分
	std::string fileNameBeforeSplit;
	// 最後の「/」または「\\」で区切られた文字列の後半部分
	std::string fileNameAfterSpilt;
};
