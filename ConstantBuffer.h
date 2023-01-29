#pragma once

#include <wrl/client.h>
#include <d3d11.h>

struct ConstantBufferBase {
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �萔�o�b�t�@�[�̍쐬
	virtual HRESULT CreateBuffer(ID3D11Device* device) = 0;

	// �萔�o�b�t�@�[
	ComPtr<ID3D11Buffer> constantBuffer = nullptr;
	ID3D11Buffer* GetConstantBuffer() const { return constantBuffer.Get(); }
};

// �]������f�[�^�ƃX���b�g�̃I�t�Z�b�g���w�肷��
template<UINT startSlot, class T>
struct ConstantBufferObject : ConstantBufferBase {
	// �萔�o�b�t�@�[�̍쐬
	HRESULT CreateBuffer(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC bufferDesc = { };
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		return device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	}

	// �]������\���̂̃f�[�^
	T data = {};
};
