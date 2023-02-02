#include "FBXMeshFile.h"

// ファイルの読み込み
bool FbxMeshFile::Load(const char* filename, ID3D11Device* device)
{
	char filepath[256];
	return true;
}

// メッシュ数の取得
UINT FbxMeshFile::GetNumMesh() const
{
	return numMesh;
}

// ポリゴン数の取得
UINT FbxMeshFile::GetNumPolegon() const
{
	return numPolygon;
}

// ファイルからメッシュの生成を行う
bool FbxMeshFile::GenerateMeshFromFile(const char* filename)
{
	// FbxManagerの作成
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) {
		return false;
	}

	// FbxImporterの作成
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return false;
	}

	// FbxSceneの作成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	// ファイル名から初期化する
	fbxImporter->Initialize(filename);
	fbxImporter->Import(fbxScene);

	// マテリアル単位でメッシュを分割する
	FbxGeometryConverter converter(fbxManager);
	converter.SplitMeshesPerMaterial(fbxScene, true);
	converter.Triangulate(fbxScene, true);

	numMesh = fbxScene->GetSrcObjectCount<FbxMesh>();

	// int materialNum = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	// for (int i = 0; i < materialNum; i++) {
	// 
	// }


	for (int i = 0; i < 1; i++) {
		CreateMesh(fbxScene->GetSrcObject<FbxMesh>(i));
	}

	fbxScene->Destroy();
	fbxImporter->Destroy();
	fbxManager->Destroy();
	return true;
}

// マテリアルデータの読み込み
void FbxMeshFile::LoadMaterial(FbxSurfaceMaterial* material)
{
	enum class Material {
		Ambient,
		Diffuse,
		Specular,
		Max,
	};

	FbxDouble3 colors[(int)Material::Max];
	FbxDouble factors[(int)Material::Max];
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sAmbient);

	if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
		const char* elementCheckList[] = {
			FbxSurfaceMaterial::sAmbient,
			FbxSurfaceMaterial::sDiffuse,
		};

		const char* factorCheckList[] = {
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sDiffuseFactor
		};

		// 対応しているマテリアルがあれば設定する
		for (int i = 0; i < 2; i++) {
			property = material->FindProperty(elementCheckList[i]);
			if (property.IsValid()) {
				colors[i] = property.Get<FbxDouble3>();
			}
			else {
				colors[i] = FbxDouble3(1.0, 1.0, 1.0);
			}

			property = material->FindProperty(factorCheckList[i]);
			if (property.IsValid()) {
				factors[i] = property.Get<FbxDouble>();
			}
			else {
				factors[i] = 1.0f;
			}
		}
	}

	FbxDouble3 ambientColor = colors[(int)Material::Ambient];
	FbxDouble ambientFactor = factors[(int)Material::Ambient];


}

// メッシュデータの作成
void FbxMeshFile::CreateMesh(FbxMesh* mesh)
{
	LoadIndices(mesh);
}


// 頂点インデックスデータを読み込む
void FbxMeshFile::LoadIndices(FbxMesh* mesh)
{
	numPolygon = mesh->GetPolygonCount();
	for (int i = 0; i < numPolygon; i++) {

	}
}
