#include "Game.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

using namespace Microsoft::WRL;

// 頂点シェーダーを表す新しいインスタンスの作成
VertexShader* VertexShader::Create(ID3D11Device* device)
{
	// 戻り値として返すインスタンスの作成
	auto result = new VertexShader();
	if (result == nullptr) {
		return nullptr;
	}

	// 頂点シェーダーの作成
	auto hr = device->CreateVertexShader(g_VertexShader, _countof(g_VertexShader), NULL, &result->shader);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// バイトコードの取得
const BYTE* VertexShader::GetBytecode()
{
	return g_VertexShader;
}

// バイトコードのサイズの取得
SIZE_T VertexShader::GetBytecodeLength()
{
	return _countof(g_VertexShader);
}

// 頂点シェーダーのネイティブポインターの取得
ID3D11VertexShader* VertexShader::GetNativePointer()
{
	return shader.Get();
}

// リソースの解放
void VertexShader::Release()
{
	shader.Reset();
	delete this;
}

// ジオメトリシェーダーを表す新しいインスタンスの作成
GeometryShader* GeometryShader::Create(ID3D11Device* device)
{
	// 戻り値として返すインスタンスの作成
	auto result = new GeometryShader();
	if (result == nullptr) {
		return nullptr;
	}

	// ジオメトリシェーダーの作成
	auto hr = device->CreateGeometryShader(g_GeometryShader, _countof(g_GeometryShader), NULL, &result->shader);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// ジオメトリシェーダーのネイティブポインターの取得
ID3D11GeometryShader* GeometryShader::GetNativePointer()
{
	return shader.Get();
}

// リソースの解放
void GeometryShader::Release()
{
	shader.Reset();
	delete this;
}

// ピクセルシェーダーを表す新しいインスタンスの作成
PixelShader* PixelShader::Create(ID3D11Device* device)
{
	// 戻り値として返すインスタンスの作成
	auto result = new PixelShader();
	if (result == nullptr) {
		return nullptr;
	}

	// ピクセルシェーダーの作成
	auto hr = device->CreatePixelShader(g_PixelShader, _countof(g_PixelShader), NULL, &result->shader);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// ピクセルシェーダーのネイティブポインターの取得
ID3D11PixelShader* PixelShader::GetNativePointer()
{
	return shader.Get();
}

// リソースの解放
void PixelShader::Release()
{
	shader.Reset();
	delete this;
}
