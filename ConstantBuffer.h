#pragma once

#include <wrl/client.h>
#include <d3d11.h>

struct ConstantBufferBase {
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 定数バッファーの作成
	virtual HRESULT CreateBuffer(ID3D11Device* device) = 0;

	// 定数バッファー
	ComPtr<ID3D11Buffer> constantBuffer = nullptr;
	ID3D11Buffer* GetConstantBuffer() const { return constantBuffer.Get(); }
};

// 転送するデータとスロットのオフセットを指定する
template<UINT startSlot, class T>
struct ConstantBufferObject : ConstantBufferBase {
	// 定数バッファーの作成
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

	// 転送する構造体のデータ
	T data = {};
};
