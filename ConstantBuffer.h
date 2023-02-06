#pragma once

#include <wrl/client.h>
#include <d3d11.h>

// 定数バッファーを管理する抽象クラス
class ConstantBufferBase
{
public:
	// 定数バッファーの作成
	virtual void CreateBuffer(ID3D11Device* device) = 0;
	// シェーダーに定数バッファーのバインド
	virtual void BindVS(ID3D11DeviceContext* immediateContext) = 0;
	virtual void BindGS(ID3D11DeviceContext* immediateContext) = 0;
	virtual void BindPS(ID3D11DeviceContext* immediateContext) = 0;
	// データの転送
	virtual void UpdateBuffer(ID3D11DeviceContext* immediateContext) = 0;

protected:
	// 定数バッファー
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer = nullptr;
};

// 転送するデータを保持する定数バッファー
template<UINT startSlot, class T>
class ConstantBufferObject : public ConstantBufferBase
{
public:
	// 転送するデータの取得
	T& GetData();
	// 定数バッファーの作成
	void CreateBuffer(ID3D11Device* device) override;
	// 頂点シェーダーにバインドする
	void BindVS(ID3D11DeviceContext* immediateContext) override;
	// ジオメトリシェーダーにバインドする
	void BindGS(ID3D11DeviceContext* immediateContext) override;
	// ピクセルシェーダーにバインドする
	void BindPS(ID3D11DeviceContext* immediateContext) override;
	// データの転送
	void UpdateBuffer(ID3D11DeviceContext* immediateContext) override;

private:
	// 転送する構造体のデータ
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
	const auto hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"定数バッファーの作成に失敗\n");
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
