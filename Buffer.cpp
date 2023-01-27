#include "Game.h"

using namespace Microsoft::WRL;

//=============================================================================
// ���_�o�b�t�@�[
//=============================================================================

// ���_�o�b�t�@�[��\���V�����C���X�^���X�̍쐬
VertexBuffer::VertexBuffer(ID3D11Device* device, UINT byteWidth)
{
	// ���_�o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// ���_�o�b�t�@�[�̍쐬
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &buffer);
	if (FAILED(hr)) {
		buffer = nullptr;
	}
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
void VertexBuffer::SetData(void* data)
{
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// �l�C�e�B�u�|�C���^�[�̎擾
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// ���\�[�X�̉��
void VertexBuffer::Release()
{
	delete this;
}

//=============================================================================
// �C���f�b�N�X�o�b�t�@�[
//=============================================================================

// �C���f�b�N�X�o�b�t�@�[��\���V�����C���X�^���X�̍쐬
IndexBuffer::IndexBuffer(ID3D11Device* device, UINT indexCount)
{
	// �C���f�b�N�X�o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(UINT16) * indexCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &buffer);
	if (FAILED(hr)) {
		buffer = nullptr;
	}
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
void IndexBuffer::SetData(UINT16* data)
{
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// �l�C�e�B�u�|�C���^�[�̎擾
ID3D11Buffer* IndexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// ���\�[�X�̉��
void IndexBuffer::Release()
{
	delete this;
}

//=============================================================================
// �萔�o�b�t�@�[
//=============================================================================

// �萔�o�b�t�@�[��\���V�����C���X�^���X�̍쐬
ConstantBuffer::ConstantBuffer(ID3D11Device* device, UINT byteWidth)
{
	// 16�o�C�g�ɓ��ꂷ��
	if (byteWidth % 16 != 0) {
		byteWidth = byteWidth + 16 - byteWidth % 16;
	}

	// �萔�o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �萔�o�b�t�@�[�̍쐬
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &buffer);
	if (FAILED(hr)) {
		buffer = nullptr;
	}
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
void ConstantBuffer::SetData(void* data)
{
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// �l�C�e�B�u�|�C���^�[�̎擾
ID3D11Buffer* ConstantBuffer::GetNativePointer()
{
	return buffer.Get();
}

// ���\�[�X�̉��
void ConstantBuffer::Release()
{
	delete this;
}
