#pragma once

#include <wrl/client.h>
#include <d3d11.h>

struct ConstantBufferBase {
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �萔�o�b�t�@�[�̍쐬
	virtual HRESULT CreateBuffer(ID3D11Device* device) = 0;

	virtual void BindVS(ID3D11DeviceContext* immediateContext) = 0;
	virtual void BindGS(ID3D11DeviceContext* immediateContext) = 0;
	virtual void BindPS(ID3D11DeviceContext* immediateContext) = 0;

	// �f�[�^�̓]��
	virtual void UpdateBuffer(ID3D11DeviceContext* immediateContext) = 0;

	// �萔�o�b�t�@�[
	ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};

// �]������f�[�^�ƃX���b�g�̃I�t�Z�b�g���w�肷��
template<UINT startSlot, class T>
struct ConstantBufferObject : ConstantBufferBase {
	// �萔�o�b�t�@�[�̍쐬
	HRESULT CreateBuffer(ID3D11Device* device)
	{
		UINT byteWidth = sizeof(T);
		// 16�o�C�g�ɓ��ꂷ��
		if (byteWidth % 16 != 0) {
			byteWidth = byteWidth + 16 - byteWidth % 16;
		}

		D3D11_BUFFER_DESC bufferDesc = { };
		bufferDesc.ByteWidth = byteWidth;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		return device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	}
	
	// ���_�V�F�[�_�[�Ƀo�C���h����
	void BindVS(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->VSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
	}

	// �W�I���g���V�F�[�_�[�Ƀo�C���h����
	void BindGS(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->GSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
	}

	// �s�N�Z���V�F�[�_�[�Ƀo�C���h����
	void BindPS(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->PSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
	}

	// �f�[�^�̓]��
	void UpdateBuffer(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &data, 0, 0);
	}

	// �]������\���̂̃f�[�^
	T data = {};
};
