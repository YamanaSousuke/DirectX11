#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <map>
#include "Vertex.h"
#include "Lightings.h"

// ���f��
class Model
{
public:
	Model() = default;
	~Model() = default;
	
	// Model(const Model&) = delete;
	// Model& operator=(const Model&) = delete;

	Model(const Model&) = default;
	Model& operator=(const Model&) = default;
	
	Model(const Model&&) = delete;
	Model& operator=(const Model&&) = delete;

	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ���b�V���f�[�^
	struct MeshData {
		// ���_�z��
		std::vector<VertexPositionNormalTextureColor> vertices = {};
		// �C���f�b�N�X�z��
		std::vector<UINT> indices = {};
		// �}�e���A���̖��O
		std::string materialName = {};
		// �}�e���A��
		Material material = {};
		// �e�N�X�`���[�̖��O
		std::string textureName = {};
		// �e�N�X�`���[
		ComPtr<ID3D11ShaderResourceView> texture = nullptr;
		// ���_�o�b�t�@�[
		ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
		// �C���f�b�N�X�o�b�t�@�[
		ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	};

	// ���f�����̐ݒ�
	void SetModelName(const std::string& name);
	// ���f�����̎擾
	const char* GetModelName() const;
	// ���b�V���f�[�^��z��ɒǉ�
	void PushMeshData(const MeshData& meshData);
	// ���b�V���f�[�^�̎擾
	std::vector<MeshData>& GetMeshData();
private:
	// ���f���̖��O
	std::string modelName;
	// ���b�V���f�[�^�̔z��
	std::vector<MeshData> meshList = {};
};
