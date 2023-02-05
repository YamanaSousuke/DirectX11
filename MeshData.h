#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <map>
#include "Vertex.h"
#include "Lightings.h"

// ���b�V���f�[�^
struct MeshData
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ���_�o�b�t�@�[
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	// �C���f�b�N�X�o�b�t�@�[
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;

	// ���_��
	UINT vertexCount = 0;
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

	// �e�N�X�`���[���̐ݒ�
	void SetTextureName(const std::string& name);
	// �e�N�X�`���[�̖��O�̎擾
	std::string GetTextureName() const;
	// �e�N�X�`���[���L�����ǂ���
	bool IsValidTexture() const;

private:
	// �e�N�X�`���[���L�����ǂ���
	bool isValidTexture = false;
};
