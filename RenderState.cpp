#include "RenderState.h"

using namespace Microsoft::WRL;

ComPtr<ID3D11SamplerState> RenderState::linerSamplerState = nullptr;

// 初期化
bool RenderState::InitAll(ID3D11Device* device)
{
	// サンプラーの作成
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	auto hr = device->CreateSamplerState(&samplerDesc, linerSamplerState.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"サンプラーの作成に失敗\n");
		return false;
	}
	return true;
}