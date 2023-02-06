#pragma once

#include <wrl/client.h>
#include <d3d11.h>

// �萔�o�b�t�@�[���Ǘ����钊�ۃN���X
class ConstantBufferBase
{
public:
	// �萔�o�b�t�@�[�̍쐬
	virtual void CreateBuffer(ID3D11Device* device) = 0;
	// �V�F�[�_�[�ɒ萔�o�b�t�@�[�̃o�C���h
	virtual void BindVS(ID3D11DeviceContext* immediateContext) = 0;
	virtual void BindGS(ID3D11DeviceContext* immediateContext) = 0;
	virtual void BindPS(ID3D11DeviceContext* immediateContext) = 0;
	// �f�[�^�̓]��
	virtual void UpdateBuffer(ID3D11DeviceContext* immediateContext) = 0;

protected:
	// �萔�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};

// �]������f�[�^��ێ�����萔�o�b�t�@�[
template<UINT startSlot, class T>
class ConstantBufferObject : public ConstantBufferBase
{
public:
	// �]������f�[�^�̎擾
	T& GetData();
	// �萔�o�b�t�@�[�̍쐬
	void CreateBuffer(ID3D11Device* device) override;
	// ���_�V�F�[�_�[�Ƀo�C���h����
	void BindVS(ID3D11DeviceContext* immediateContext) override;
	// �W�I���g���V�F�[�_�[�Ƀo�C���h����
	void BindGS(ID3D11DeviceContext* immediateContext) override;
	// �s�N�Z���V�F�[�_�[�Ƀo�C���h����
	void BindPS(ID3D11DeviceContext* immediateContext) override;
	// �f�[�^�̓]��
	void UpdateBuffer(ID3D11DeviceContext* immediateContext) override;

private:
	// �]������\���̂̃f�[�^
	T data = {};
};

template<UINT startSlot, class T>
T& ConstantBufferObject<startSlot, T>::GetData()
{
	return data;
}

template<UINT startSlot, class T>
void ConstantBufferObject<startSlot, T>::CreateBuffer(ID3D11Device* device)
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
	const auto hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"�萔�o�b�t�@�[�̍쐬�Ɏ��s\n");
	}
}

template<UINT startSlot, class T>
void ConstantBufferObject<startSlot, T>::BindVS(ID3D11DeviceContext* immediateContext)
{
	immediateContext->VSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
}

template<UINT startSlot, class T>
void ConstantBufferObject<startSlot, T>::BindGS(ID3D11DeviceContext* immediateContext)
{
	immediateContext->GSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
}

template<UINT startSlot, class T>
void ConstantBufferObject<startSlot, T>::BindPS(ID3D11DeviceContext* immediateContext)
{
	immediateContext->PSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
}

template<UINT startSlot, class T>
void ConstantBufferObject<startSlot, T>::UpdateBuffer(ID3D11DeviceContext* immediateContext)
{
	immediateContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &data, 0, 0);
}
