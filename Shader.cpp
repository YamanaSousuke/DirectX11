#include "Game.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

using namespace Microsoft::WRL;

//=============================================================================
// 頂点シェーダー
//=============================================================================

// 頂点シェーダーを表す新しいインスタンスの作成
VertexShader::VertexShader(ID3D11Device* device)
{
	// 頂点シェーダーの作成
	auto hr = device->CreateVertexShader(g_VertexShader, _countof(g_VertexShader), NULL, &shader);
	if (FAILED(hr)) {
		shader = nullptr;
	}
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
	delete this;
}

//=============================================================================
// ジオメトリシェーダー
//=============================================================================

// ジオメトリシェーダーを表す新しいインスタンスの作成
GeometryShader::GeometryShader(ID3D11Device* device)
{
	// ジオメトリシェーダーの作成
	auto hr = device->CreateGeometryShader(g_GeometryShader, _countof(g_GeometryShader), NULL, &shader);
	if (FAILED(hr)) {
		shader = nullptr;
	}
}

// ジオメトリシェーダーのネイティブポインターの取得
ID3D11GeometryShader* GeometryShader::GetNativePointer()
{
	return shader.Get();
}

// リソースの解放
void GeometryShader::Release()
{
	delete this;
}

//=============================================================================
// ピクセルシェーダー
//=============================================================================

// ピクセルシェーダーを表す新しいインスタンスの作成
PixelShader::PixelShader(ID3D11Device* device)
{
	// ピクセルシェーダーの作成
	auto hr = device->CreatePixelShader(g_PixelShader, _countof(g_PixelShader), NULL, &shader);
	if (FAILED(hr)) {
		shader = nullptr;
	}
}

// ピクセルシェーダーのネイティブポインターの取得
ID3D11PixelShader* PixelShader::GetNativePointer()
{
	return shader.Get();
}

// リソースの解放
void PixelShader::Release()
{
	delete this;
}
