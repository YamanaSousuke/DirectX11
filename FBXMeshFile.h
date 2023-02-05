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
	// �`��
	void Draw(ID3D11DeviceContext* immediateContext);
	// ���b�V�����̎擾
	size_t GetMeshCount() const;
	// ���b�V���f�[�^�̎擾
	MeshData GetMeshData(int index) const;
	// �V�F�[�_�[���\�[�X�r���[�̎擾
	ID3D11ShaderResourceView* GetShaderResourceView(int index) const;
	// �}�e���A���̎擾
	Material GetMaterial(int index) const;


	// �p�����[�^�[�̎擾
	std::string GetFbxFileName() const;
	std::vector<std::string> GetTextureName() const;
	std::string GetTextureName(int index) const;
	UINT GetMaterialCount() const;
	UINT GetTextureCount() const;

	struct ModelData {
		DirectX::XMMATRIX world;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffues;
		DirectX::XMFLOAT4 specular;
	};
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �t�@�C�����烁�b�V���̐������s��
	bool GenerateMeshFromFile(const std::string& filename);
	// �}�e���A���f�[�^�̓ǂݍ���
	void LoadMaterial(FbxSurfaceMaterial* material);
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
	// �e�N�X�`���[����ǂݍ���
	bool LoadTexture(FbxFileTexture* textrue, std::string& keyword);
	// UV���W�̓ǂݍ���
	void LoadUV(MeshData& meshData, FbxMesh* mesh);

	// ���_�o�b�t�@�[�̍쐬
	bool CreateVertexBuffer(ID3D11DeviceContext* immediateContext);
	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	bool CreateIndexBuffer(ID3D11DeviceContext* immediateContext);

	// ���b�V���f�[�^
	std::vector<MeshData> meshList = {};
	// �}�e���A��
	std::map<std::string, Material> materials = {};

	ComPtr<ID3D11Device> device = nullptr;
	// �C���v�b�g���C�A�E�g
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	// �萔�o�b�t�@�[
	ComPtr<ID3D11Buffer> constantBuffer = nullptr;

	std::map<std::string, ComPtr<ID3D11ShaderResourceView>> texture;
	std::map<std::string, ComPtr<ID3D11ShaderResourceView>> materialLinks;
	std::vector<std::string> textureNamess;

	// �Ō�́u/�v�܂��́u\\�v�ŋ�؂�ꂽ������̑O������
	std::string fileNameBeforeSplit;
	// �Ō�́u/�v�܂��́u\\�v�ŋ�؂�ꂽ������̌㔼����
	std::string fileNameAfterSpilt;
	UINT materialCount = 0;
};
