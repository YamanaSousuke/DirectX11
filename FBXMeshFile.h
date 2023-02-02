#pragma once

#include <fbxsdk.h>
#include <d3d11.h>
#include <wrl/client.h>

class FbxMeshFile
{
public:
	// �t�@�C���̓ǂݍ���
	bool Load(const char* filename, ID3D11Device* device);
	// �t�@�C�����烁�b�V���̐������s��
	bool GenerateMeshFromFile(const char* filename);
	// ���b�V�����̎擾
	UINT GetNumMesh() const;
	// �|���S�����̎擾
	UINT GetNumPolegon() const;
private:
	// templ
	// using ComPtr = Microsoft::WRL::ComPtr<T>


	// �}�e���A���f�[�^�̓ǂݍ���
	void LoadMaterial(FbxSurfaceMaterial* material);
	// ���b�V���f�[�^�̍쐬
	void CreateMesh(FbxMesh* mesh);
	// ���_�C���f�b�N�X�f�[�^��ǂݍ���
	void LoadIndices(FbxMesh* mesh);

	// ���b�V����
	int numMesh = 0;
	// �|���S����
	int numPolygon = 0;
};
