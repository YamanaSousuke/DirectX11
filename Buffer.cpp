#include "Game.h"

using namespace Microsoft::WRL;

// ���_�o�b�t�@�[��\���V�����C���X�^���X�̍쐬
VertexBuffer* VertexBuffer::Create(ID3D11Device* device, UINT byteWidth)
{
	// ���̃N���X�̃������[���m��
	auto result = new VertexBuffer();
	if (result == nullptr) {
		return nullptr;
	}

	// ���_�o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// ���_�o�b�t�@�[�̍쐬
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &result->buffer);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
void VertexBuffer::SetData(void* data)
{
	// �f�o�C�X�̎擾
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	// �f�o�C�X�R���e�L�X�g�̎擾
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	// �o�b�t�@�[�Ƀf�[�^��]��
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	device.Reset();
	deviceContext.Reset();
}

// �l�C�e�B�u�|�C���^�[�̎擾
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// ���\�[�X�̉��
void VertexBuffer::Release()
{
	buffer.Reset();
	delete this;
}

// �C���f�b�N�X�o�b�t�@�[��\���V�����C���X�^���X�̍쐬
IndexBuffer* IndexBuffer::Create(ID3D11Device* device, UINT indexCount)
{
	// ���̃N���X�̃������[���m��
	auto result = new IndexBuffer();
	if (result == nullptr) {
		return nullptr;
	}

	// �C���f�b�N�X�o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(UINT32) * indexCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &result->buffer);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
void IndexBuffer::SetData(UINT32* data)
{
	// �f�o�C�X�̎擾
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	// �f�o�C�X�R���e�L�X�g�̎擾
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	// �o�b�t�@�[�Ƀf�[�^��]��
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	device.Reset();
	deviceContext.Reset();
}

// �l�C�e�B�u�|�C���^�[�̎擾
ID3D11Buffer* IndexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// ���\�[�X�̉��
void IndexBuffer::Release()
{
	buffer.Reset();
	delete this;
}

// �萔�o�b�t�@�[��\���V�����C���X�^���X�̍쐬
ConstantBuffer* ConstantBuffer::Create(ID3D11Device* device, UINT byteWidth)
{
	// ���̃N���X�̃������[���m��
	auto result = new ConstantBuffer();
	if (result == nullptr) {
		return nullptr;
	}

	// �C���f�b�N�X�o�b�t�@�[�ɂ��Ă̋L�q
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// �萔�o�b�t�@�[�̍쐬
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &result->buffer);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
void ConstantBuffer::SetData(void* data)
{
	// �f�o�C�X�̎擾
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	// �f�o�C�X�R���e�L�X�g�̎擾
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	// �o�b�t�@�[�Ƀf�[�^��]��
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	device.Reset();
	deviceContext.Reset();
}

// �l�C�e�B�u�|�C���^�[�̎擾
ID3D11Buffer* ConstantBuffer::GetNativePointer()
{
	return buffer.Get();
}

// ���\�[�X�̉��
void ConstantBuffer::Release()
{
	buffer.Reset();
	delete this;
}
