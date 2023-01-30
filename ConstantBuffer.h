#pragma once

#include <wrl/client.h>
#include <d3d11.h>

struct ConstantBufferBase {
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 定数バッファーの作成
	virtual HRESULT CreateBuffer(ID3D11Device* device) = 0;

	virtual void BindVS(ID3D11DeviceContext* immediateContext) = 0;
	virtual void BindGS(ID3D11DeviceContext* immediateContext) = 0;
	virtual void BindPS(ID3D11DeviceContext* immediateContext) = 0;

	// データの転送
	virtual void UpdateBuffer(ID3D11DeviceContext* immediateContext) = 0;

	// 定数バッファー
	ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};

// 転送するデータとスロットのオフセットを指定する
template<UINT startSlot, class T>
struct ConstantBufferObject : ConstantBufferBase {
	// 定数バッファーの作成
	HRESULT CreateBuffer(ID3D11Device* device)
	{
		UINT byteWidth = sizeof(T);
		// 16バイトに統一する
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
	
	// 頂点シェーダーにバインドする
	void BindVS(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->VSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
	}

	// ジオメトリシェーダーにバインドする
	void BindGS(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->GSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
	}

	// ピクセルシェーダーにバインドする
	void BindPS(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->PSSetConstantBuffers(startSlot, 1, constantBuffer.GetAddressOf());
	}

	// データの転送
	void UpdateBuffer(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &data, 0, 0);
	}

	// 転送する構造体のデータ
	T data = {};
};
