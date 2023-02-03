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
	// �t�@�C���̓ǂݍ���
	bool Load(const char* filename, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	// �`��
	void Draw(ID3D11DeviceContext* immediateContext);

	struct ModelData {
		DirectX::XMMATRIX world;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffues;
		DirectX::XMFLOAT4 specular;
	};
private:
	// �t�@�C�����烁�b�V���̐������s��
	bool GenerateMeshFromFile(const char* filename);
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

	// ���_�o�b�t�@�[�̍쐬
	bool CreateVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	bool CreateIndexBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext);

	// ���b�V���f�[�^
	std::vector<MeshData> meshList = {};
	// �}�e���A��
	std::map<std::string, Material> materials = {};

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	// �萔�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};
