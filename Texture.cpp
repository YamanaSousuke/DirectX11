#include "Game.h"

using namespace Microsoft::WRL;

// このクラスの新しいインスタンスの作成
Texture2D::Texture2D(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool mipChain)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = mipChain ? 0 : 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	auto hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
	if (FAILED(hr)) {
	}

	// サンプラーステートの作成
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(hr)) {
		// return nullptr;
	}

	// テクスチャ用のシェーダーリソースビューを作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mipChain ? 0 : 1;
	hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, &shaderResourceView);
	if (FAILED(hr)) {
		// return nullptr;
	}
}

// バッファーにデータを設定する
void Texture2D::SetData(const void* data)
{
	ComPtr<ID3D11Device> device;
	texture->GetDevice(&device);
	ComPtr<ID3D11DeviceContext> deviceContext;
	device->GetImmediateContext(&deviceContext);
	deviceContext->UpdateSubresource(texture.Get(), 0, NULL, data, 4 * sizeof(uint32_t), 0);
}

// ネイティブポインターの取得
ID3D11Texture2D* Texture2D::GetNativePointer()
{
	return texture.Get();
}

// ネイティブポインターの取得
ID3D11SamplerState* Texture2D::GetSapmlerState()
{
	return samplerState.Get();
}

// ネイティブポインターの取得
ID3D11ShaderResourceView* Texture2D::GetShaderResourceView()
{
	return shaderResourceView.Get();
}

// リソースの解放
void Texture2D::Release()
{
	shaderResourceView.Reset();
	samplerState.Reset();
	texture.Reset();
	delete this;
}
