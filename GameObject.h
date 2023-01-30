#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include "Transform.h"
#include "Geometry.h"
#include "Lightings.h"
#include "Effect.h"

// �Q�[���I�u�W�F�N�g
class GameObject 
{
public:
	// ���_�o�b�t�@�[�ƃC���f�b�N�X�o�b�t�@�[�̐ݒ�
	template<class VertexType>
	void SetBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext, Geometry::Meshdata<VertexType> meshdata);
	// �g�����X�t�H�[���̎擾
	Transform& GetTransform();
	// �e�N�X�`���̐ݒ�
	void SetTexture(ID3D11ShaderResourceView* texture);
	// �}�e���A���̐ݒ�
	void SetMaterial(const Material& material);
	// �`��
	void Draw(ID3D11DeviceContext* immediateContext, Effect& effect);
	// ���\�[�X�̉��
	void Release();

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �g�����X�t�H�[��
	Transform transform = {};
	// ���_�o�b�t�@�[
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	// �C���f�b�N�X�o�b�t�@�[
	ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	// �e�N�X�`���[
	ComPtr<ID3D11ShaderResourceView> texture = nullptr;
	// �}�e���A��
	Material material = {};
	// 1���_�̃T�C�Y
	UINT vertexStride = 0;
	// �C���f�b�N�X�̌�
	UINT indexCount = 0;
};

template<class VertexType>
void GameObject::SetBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext, Geometry::Meshdata<VertexType> meshdata)
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
	auto hr = device->CreateBuffer(&vertexBufferDesc, nullptr, vertexBuffer.GetAddressOf());
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
	hr = device->CreateBuffer(&indexBufferDesc, nullptr, &indexBuffer);
	if (FAILED(hr)) {
		OutputDebugString(L"�C���f�b�N�X�o�b�t�@�[�̍쐬�Ɏ��s\n");
	}
	immediateContext->UpdateSubresource(indexBuffer.Get(), 0, nullptr, meshdata.indices.data(), 0, 0);
}
