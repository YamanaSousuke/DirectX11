#include "FBXMeshFile.h"

// �t�@�C���̓ǂݍ���
bool FbxMeshFile::Load(const char* filename, ID3D11Device* device)
{
	char filepath[256];
	return true;
}

// ���b�V�����̎擾
UINT FbxMeshFile::GetNumMesh() const
{
	return numMesh;
}

// �|���S�����̎擾
UINT FbxMeshFile::GetNumPolegon() const
{
	return numPolygon;
}

// �t�@�C�����烁�b�V���̐������s��
bool FbxMeshFile::GenerateMeshFromFile(const char* filename)
{
	// FbxManager�̍쐬
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) {
		return false;
	}

	// FbxImporter�̍쐬
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return false;
	}

	// FbxScene�̍쐬
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	// �t�@�C�������珉��������
	fbxImporter->Initialize(filename);
	fbxImporter->Import(fbxScene);

	// �}�e���A���P�ʂŃ��b�V���𕪊�����
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

// �}�e���A���f�[�^�̓ǂݍ���
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

		// �Ή����Ă���}�e���A��������ΐݒ肷��
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

// ���b�V���f�[�^�̍쐬
void FbxMeshFile::CreateMesh(FbxMesh* mesh)
{
	LoadIndices(mesh);
}


// ���_�C���f�b�N�X�f�[�^��ǂݍ���
void FbxMeshFile::LoadIndices(FbxMesh* mesh)
{
	numPolygon = mesh->GetPolygonCount();
	for (int i = 0; i < numPolygon; i++) {

	}
}
