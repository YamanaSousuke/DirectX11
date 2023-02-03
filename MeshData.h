#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include "Vertex.h"

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
	std::vector<VertexPosition> vertices = {};
	// �C���f�b�N�X�z��
	std::vector<UINT> indices = {};
	// �}�e���A���̖��O
	std::string materialName = {};
};
