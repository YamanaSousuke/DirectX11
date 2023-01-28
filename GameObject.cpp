#include "GameObject.h"

using namespace DirectX;

// ���_�o�b�t�@�[�ƃC���f�b�N�X�o�b�t�@�[�̐ݒ�
//template<class VertexType>
//inline void GameObject::SetBuffer(ID3D11Device* device, ID3D11DeviceContext* immediateContext, Geometry::Meshdata<VertexType> meshdata)
//{
//	vertexStride = sizeof(VertexType);
//	indexCount = meshdata.indices.size();
//
//	// ���_�o�b�t�@�[�̍쐬
//	D3D11_BUFFER_DESC vertexBufferDesc = {};
//	vertexBufferDesc.ByteWidth = vertexStride * meshdata.vertices.size();
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//	auto hr = device->CreateBuffer(&vertexBufferDesc, NULL, vertexBuffer.GetAddressOf());
//	if (FAILED(hr)) {
//		OutputDebugString(L"���_�o�b�t�@�[�̍쐬�Ɏ��s\n");
//	}
//	immediateContext->UpdateSubresource(vertexBuffer, 0, nullptr, meshdata.vertices.data(), 0, 0);
//
//	// �C���f�b�N�X�o�b�t�@�[�̍쐬
//	D3D11_BUFFER_DESC indexBufferDesc = {};
//	indexBufferDesc.ByteWidth = sizeof(UINT16) * indexCount;
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//	indexBufferDesc.StructureByteStride = 0;
//	auto hr = device->CreateBuffer(&indexBufferDesc, NULL, &indexBuffer);
//	if (FAILED(hr)) {
//		OutputDebugString(L"�C���f�b�N�X�o�b�t�@�[�̍쐬�Ɏ��s\n");
//	}
//	immediateContext->UpdateSubresource(indexBuffer, 0, nullptr, meshdata.indices.data(), 0, 0);
//}

// �`��
void GameObject::Draw(ID3D11DeviceContext* immediateContext, const XMFLOAT3& position)
{
	// ���_�o�b�t�@�[�ƃC���f�b�N�X�o�b�t�@�[�̐ݒ�
	ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get()};
	UINT strides[1] = { vertexStride };
	UINT offsets[1] = { 0 };
	immediateContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBuffers), vertexBuffers, strides, offsets);
	immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	ModelParameter modelParameter = {};
	static float time = 0.0f;
	time += 0.01666f;

	// ���[���h�s��
	XMMATRIX world = XMMatrixIdentity();
	XMVECTOR axis = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
	world *= XMMatrixRotationAxis(axis, time);
	world *= XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&modelParameter.world, XMMatrixTranspose(world));

	ComPtr<ID3D11Buffer> constnatBuffer = nullptr;
	immediateContext->GSGetConstantBuffers(1, 1, constnatBuffer.GetAddressOf());
	immediateContext->UpdateSubresource(constnatBuffer.Get(), 0, nullptr, &modelParameter, 0, 0);
	immediateContext->DrawIndexed(indexCount, 0, 0);
}

// ���\�[�X�̉��
void GameObject::Release()
{
	delete this;
}
