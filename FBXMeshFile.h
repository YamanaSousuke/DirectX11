#pragma once

#include <fbxsdk.h>
#include <d3d11.h>
#include <wrl/client.h>

class FbxMeshFile
{
public:
	// ファイルの読み込み
	bool Load(const char* filename, ID3D11Device* device);
	// ファイルからメッシュの生成を行う
	bool GenerateMeshFromFile(const char* filename);
	// メッシュ数の取得
	UINT GetNumMesh() const;
	// ポリゴン数の取得
	UINT GetNumPolegon() const;
private:
	// templ
	// using ComPtr = Microsoft::WRL::ComPtr<T>


	// マテリアルデータの読み込み
	void LoadMaterial(FbxSurfaceMaterial* material);
	// メッシュデータの作成
	void CreateMesh(FbxMesh* mesh);
	// 頂点インデックスデータを読み込む
	void LoadIndices(FbxMesh* mesh);

	// メッシュ数
	int numMesh = 0;
	// ポリゴン数
	int numPolygon = 0;
};
