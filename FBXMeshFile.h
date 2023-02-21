#pragma once
#pragma warning(disable : 26451)
#pragma warning(disable : 26495)
#pragma warning(disable : 4099)

#include <fbxsdk.h>
#include <DirectXMath.h>
#include "Model.h"

// FBX�t�@�C������ǂݍ��݂��s��
class FbxMeshFile
{
public:
	FbxMeshFile() = default;
	~FbxMeshFile() = default;

	FbxMeshFile(const FbxMeshFile&) = delete;
	FbxMeshFile& operator=(const FbxMeshFile&) = delete;

	FbxMeshFile(FbxMeshFile&&) = default;
	FbxMeshFile& operator=(FbxMeshFile&&) = default;

	// ������
	void Init(ID3D11Device* device);
	// �t�@�C���̓ǂݍ���
	Model Load(const std::string& filename, ID3D11DeviceContext* immediateContext);
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �t�@�C�����烁�b�V���̐������s��
	bool GenerateMeshFromFile(const std::string& filename);
	// �}�e���A���f�[�^�̓ǂݍ���
	void LoadMaterial(Model::MeshData& meshData, FbxMesh* mesh);
	// ���b�V���f�[�^�̍쐬
	void CreateMesh(FbxMesh* mesh);
	// ���_�C���f�b�N�X�f�[�^��ǂݍ���
	void LoadIndices(Model::MeshData& meshData, FbxMesh* mesh);
	// ���_�f�[�^��ǂݍ���
	void LoadVertices(Model::MeshData& meshData, FbxMesh* mesh);
	// �}�e���A�����̐ݒ�
	void SetMaterial(Model::MeshData& meshData, FbxMesh* mesh);
	// ���_�J���[�f�[�^��ǂݍ���
	void LoadColors(Model::MeshData& meshData, FbxMesh* mesh);
	// �@���f�[�^��ǂݍ���
	void LoadNormal(Model::MeshData& meshData, FbxMesh* mesh);
	// UV���W�̓ǂݍ���
	void LoadUV(Model::MeshData& meshData, FbxMesh* mesh);
	// ���_�o�b�t�@�[�A�C���f�b�N�X�o�b�t�@�[�̍쐬
	void CreateBuffer(ID3D11DeviceContext* immediateContext);

	// �f�o�C�X
	ComPtr<ID3D11Device> device = nullptr;
	// �Ō�́u/�v�܂��́u\\�v�ŋ�؂�ꂽ������̑O������
	std::string fileNameBeforeSplit;
	// �Ō�́u/�v�܂��́u\\�v�ŋ�؂�ꂽ������̌㔼����
	std::string fileNameAfterSpilt;
	// ���f��
	Model model = {};
};
