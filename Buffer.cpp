#include "Game.h"

using namespace Microsoft::WRL;

//=============================================================================
// 頂点バッファー
//=============================================================================

// 頂点バッファーを表す新しいインスタンスの作成
VertexBuffer::VertexBuffer(ID3D11Device* device, UINT byteWidth)
{
	// 頂点バッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 頂点バッファーの作成
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &buffer);
	if (FAILED(hr)) {
		buffer = nullptr;
	}
}

// バッファーにデータを設定する
void VertexBuffer::SetData(void* data)
{
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// ネイティブポインターの取得
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// リソースの解放
void VertexBuffer::Release()
{
	delete this;
}

//=============================================================================
// インデックスバッファー
//=============================================================================

// インデックスバッファーを表す新しいインスタンスの作成
IndexBuffer::IndexBuffer(ID3D11Device* device, UINT indexCount)
{
	// インデックスバッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(UINT16) * indexCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// インデックスバッファーの作成
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &buffer);
	if (FAILED(hr)) {
		buffer = nullptr;
	}
}

// バッファーにデータを設定する
void IndexBuffer::SetData(UINT16* data)
{
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);
}

// ネイティブポインターの取得
ID3D11Buffer* IndexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// リソースの解放
void IndexBuffer::Release()
{
	delete this;
}

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
