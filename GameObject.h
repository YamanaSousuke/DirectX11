#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include "Geometry.h"


// �Q�[���I�u�W�F�N�g
class GameObject 
{
public:
	// ���_�o�b�t�@�[�ƃC���f�b�N�X�o�b�t�@�[�̐ݒ�
	template<class VertexType>
	void SetBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext, Geometry::Meshdata<VertexType> meshdata);
	// �e�N�X�`���̐ݒ�
	void SetTexture(ID3D11ShaderResourceView* texture);
	// �`��
	void Draw(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT3& position);
	// ���\�[�X�̉��
	void Release();
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ���f�����
	struct ModelParameter {
		DirectX::XMFLOAT4X4 world;
	};

	// ���_�o�b�t�@�[
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	// �C���f�b�N�X�o�b�t�@�[
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	// �e�N�X�`���[
	ComPtr<ID3D11ShaderResourceView> texture = nullptr;
	// 1���_�̃T�C�Y
	UINT vertexStride = 0;
	// �C���f�b�N�X�̌�
	UINT indexCount = 0;
};

template<class VertexType>
inline void GameObject::SetBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext, Geometry::Meshdata<VertexType> meshdata)
{
	vertexStride = sizeof(VertexType);
	indexCount = (UINT)meshdata.indices.size();
	
	// ���_�o�b�t�@�[�̍쐬
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.ByteWidth = vertexStride * (UINT)meshdata.vertices.size();
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	auto hr = device->CreateBuffer(&vertexBufferDesc, NULL, vertexBuffer.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"���_�o�b�t�@�[�̍쐬�Ɏ��s\n");
	}
	immediateContext->UpdateSubresource(vertexBuffer.Get(), 0, nullptr, meshdata.vertices.data(), 0, 0);
	
	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.ByteWidth = sizeof(UINT16) * indexCount;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	hr = device->CreateBuffer(&indexBufferDesc, NULL, &indexBuffer);
	if (FAILED(hr)) {
		OutputDebugString(L"�C���f�b�N�X�o�b�t�@�[�̍쐬�Ɏ��s\n");
	}
	immediateContext->UpdateSubresource(indexBuffer.Get(), 0, nullptr, meshdata.indices.data(), 0, 0);
}
