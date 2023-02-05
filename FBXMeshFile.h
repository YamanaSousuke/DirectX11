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
	// ������
	void Init(ID3D11Device* device);
	// �t�@�C���̓ǂݍ���
	FbxMeshFile* Load(const std::string& filename, ID3D11DeviceContext* immediateContext);
	// ���b�V�����̎擾
	size_t GetMeshCount() const;
	// ���b�V���f�[�^�̎擾
	MeshData GetMeshData(int index) const;

	std::vector<MeshData> GetMeshData() const;
	// Fbx�t�@�C�����̎擾
	std::string GetFbxFileName() const;
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �t�@�C�����烁�b�V���̐������s��
	bool GenerateMeshFromFile(const std::string& filename);
	// �}�e���A���f�[�^�̓ǂݍ���
	void LoadMaterial(MeshData& meshData, FbxMesh* mesh);
	// ���b�V���f�[�^�̍쐬
	void CreateMesh(FbxMesh* mesh);
	// ���_�C���f�b�N�X�f�[�^��ǂݍ���
	void LoadIndices(MeshData& meshData, FbxMesh* mesh);
	// ���_�f�[�^��ǂݍ���
	void LoadVertices(MeshData& meshData, FbxMesh* mesh);
	// �}�e���A�����̐ݒ�
	void SetMaterial(MeshData& meshData, FbxMesh* mesh);
	// ���_�J���[�f�[�^��ǂݍ���
	void LoadColors(MeshData& meshData, FbxMesh* mesh);
	// �@���f�[�^��ǂݍ���
	void LoadNormal(MeshData& meshData, FbxMesh* mesh);
	// UV���W�̓ǂݍ���
	void LoadUV(MeshData& meshData, FbxMesh* mesh);

	// ���_�o�b�t�@�[�̍쐬
	bool CreateVertexBuffer(ID3D11DeviceContext* immediateContext);
	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	bool CreateIndexBuffer(ID3D11DeviceContext* immediateContext);

	// ���b�V���f�[�^
	std::vector<MeshData> meshList = {};
	// �f�o�C�X
	ComPtr<ID3D11Device> device = nullptr;
	// �Ō�́u/�v�܂��́u\\�v�ŋ�؂�ꂽ������̑O������
	std::string fileNameBeforeSplit;
	// �Ō�́u/�v�܂��́u\\�v�ŋ�؂�ꂽ������̌㔼����
	std::string fileNameAfterSpilt;
};
