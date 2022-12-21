#include "Game.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

using namespace Microsoft::WRL;

// 頂点シェーダーを表す新しいインスタンスの作成
VertexShader* VertexShader::Create(ID3D11Device* device)
{
	// 頂点シェーダーの作成
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	auto hr = device->CreateVertexShader(g_VertexShader, sizeof(g_VertexShader), NULL, &vertexShader);
	if (FAILED(hr)) {
		return nullptr;
	}

	// 戻り値として返すインスタンスの作成
	auto result = new VertexShader();
	if (result == nullptr) {
		return nullptr;
	}
	result->vertexShader = vertexShader;
	return result;
}

// 頂点シェーダーのネイティブポインターの取得
ID3D11VertexShader* VertexShader::GetNativePointer()
{
	return vertexShader.Get();
}

// リソースの解放
void VertexShader::Release()
{
	vertexShader.Reset();
	delete this;
}

// ジオメトリシェーダーを表す新しいインスタンスの作成
GeometryShader* GeometryShader::Create(ID3D11Device* device)
{
	// ジオメトリシェーダーの作成
	ComPtr<ID3D11GeometryShader> geometryShader = nullptr;
	auto hr = device->CreateGeometryShader(g_GeometryShader, sizeof(g_GeometryShader), NULL, &geometryShader);
	if (FAILED(hr)) {
		return nullptr;
	}

	// 戻り値として返すインスタンスの作成
	auto result = new GeometryShader();
	if (result == nullptr) {
		return nullptr;
	}
	result->geometryShader = geometryShader;
	return result;
}

// ジオメトリシェーダーのネイティブポインターの取得
ID3D11GeometryShader* GeometryShader::GetNativePointer()
{
	return geometryShader.Get();
}

// リソースの解放
void GeometryShader::Release()
{
	geometryShader.Reset();
	delete this;
}

// ピクセルシェーダーを表す新しいインスタンスの作成
PixelShader* PixelShader::Create(ID3D11Device* device)
{
	// ピクセルシェーダーの作成
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	auto hr = device->CreatePixelShader(g_PixelShader, sizeof(g_PixelShader), NULL, &pixelShader);
	if (FAILED(hr)) {
		return nullptr;
	}

	// 戻り値として返すインスタンスの作成
	auto result = new PixelShader();
	if (result == nullptr) {
		return nullptr;
	}
	result->pixelShader = pixelShader;
	return result;
}

// ピクセルシェーダーのネイティブポインターの取得
ID3D11PixelShader* PixelShader::GetNativePointer()
{
	return pixelShader.Get();
}

// リソースの解放
void PixelShader::Release()
{
	pixelShader.Reset();
	delete this;
}
