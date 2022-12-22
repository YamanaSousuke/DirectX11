#include "Game.h"

using namespace Microsoft::WRL;

// 頂点バッファーを表す新しいインスタンスの作成
VertexBuffer* VertexBuffer::Create(ID3D11Device* device, UINT byteWidth)
{
	// このクラスのメモリーを確保
	auto result = new VertexBuffer();
	if (result == nullptr) {
		return nullptr;
	}

	// 頂点バッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 頂点バッファーの作成
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &result->buffer);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// バッファーにデータを設定する
void VertexBuffer::SetData(void* data)
{
	// デバイスの取得
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	// デバイスコンテキストの取得
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	// バッファーにデータを転送
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	device.Reset();
	deviceContext.Reset();
}

// ネイティブポインターの取得
ID3D11Buffer* VertexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// リソースの解放
void VertexBuffer::Release()
{
	buffer.Reset();
	delete this;
}

// インデックスバッファーを表す新しいインスタンスの作成
IndexBuffer* IndexBuffer::Create(ID3D11Device* device, UINT indexCount)
{
	// このクラスのメモリーを確保
	auto result = new IndexBuffer();
	if (result == nullptr) {
		return nullptr;
	}

	// インデックスバッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(UINT32) * indexCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// インデックスバッファーの作成
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &result->buffer);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// バッファーにデータを設定する
void IndexBuffer::SetData(UINT32* data)
{
	// デバイスの取得
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	// デバイスコンテキストの取得
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	// バッファーにデータを転送
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	device.Reset();
	deviceContext.Reset();
}

// ネイティブポインターの取得
ID3D11Buffer* IndexBuffer::GetNativePointer()
{
	return buffer.Get();
}

// リソースの解放
void IndexBuffer::Release()
{
	buffer.Reset();
	delete this;
}

// 定数バッファーを表す新しいインスタンスの作成
ConstantBuffer* ConstantBuffer::Create(ID3D11Device* device, UINT byteWidth)
{
	// このクラスのメモリーを確保
	auto result = new ConstantBuffer();
	if (result == nullptr) {
		return nullptr;
	}

	// インデックスバッファーについての記述
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	// 定数バッファーの作成
	auto hr = device->CreateBuffer(&bufferDesc, NULL, &result->buffer);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// バッファーにデータを設定する
void ConstantBuffer::SetData(void* data)
{
	// デバイスの取得
	ComPtr<ID3D11Device> device = nullptr;
	buffer->GetDevice(&device);
	// デバイスコンテキストの取得
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);
	// バッファーにデータを転送
	deviceContext->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	device.Reset();
	deviceContext.Reset();
}

// ネイティブポインターの取得
ID3D11Buffer* ConstantBuffer::GetNativePointer()
{
	return buffer.Get();
}

// リソースの解放
void ConstantBuffer::Release()
{
	buffer.Reset();
	delete this;
}
