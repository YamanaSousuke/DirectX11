#pragma once

#include <fbxsdk.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <DirectXMath.h>

#include "VertexShader.h"
#include "MeshData.h"
#include "Lightings.h"
#include <map>

class FbxMeshFile
{
public:
	// ファイルの読み込み
	bool Load(const char* filename, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	// 描画
	void Draw(ID3D11DeviceContext* immediateContext);

	struct ModelData {
		DirectX::XMMATRIX world;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffues;
		DirectX::XMFLOAT4 specular;
	};
private:
	// ファイルからメッシュの生成を行う
	bool GenerateMeshFromFile(const char* filename);
	// マテリアルデータの読み込み
	void LoadMaterial(FbxSurfaceMaterial* material);
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

	// 頂点バッファーの作成
	bool CreateVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	// インデックスバッファーの作成
	bool CreateIndexBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext);

	// メッシュデータ
	std::vector<MeshData> meshList = {};
	// マテリアル
	std::map<std::string, Material> materials = {};

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	// 定数バッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};
