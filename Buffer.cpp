#include "Game.h"

using namespace Microsoft::WRL;


//=============================================================================
// 定数バッファー
//=============================================================================

// 定数バッファーを表す新しいインスタンスの作成
ConstantBuffer::ConstantBuffer(ID3D11Device* device, UINT byteWidth)
{
	// 16バイトに統一する
	if (byteWidth % 16 != 0) {
		byteWidth = byteWidth + 16 - byteWidth % 16;
	}

	// 定数バッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 定数バッファーの作成
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &buffer);
	if (FAILED(hr)) {
		buffer = nullptr;
	}
}

// バッファーにデータを設定する
void ConstantBuffer::SetData(void* data)
{
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// ネイティブポインターの取得
ID3D11Buffer* ConstantBuffer::GetNativePointer()
{
	return buffer.Get();
}

// リソースの解放
void ConstantBuffer::Release()
{
	delete this;
}
