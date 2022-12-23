#include "Game.h"

using namespace Microsoft::WRL;

// このクラスの新しいインスタンスの作成
RasterizerState* RasterizerState::Create(ID3D11Device* device)
{
	// このクラスのメモリーを確保
	auto result = new RasterizerState();
	if (result == nullptr) {
		return nullptr;
	}

	// ラスタライザステートの作成
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	auto hr = device->CreateRasterizerState(&rasterizerDesc, &result->state);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// ネイティブポインターの取得
ID3D11RasterizerState* RasterizerState::GetNativePointer()
{
	return state.Get();
}

// リソースの解放
void RasterizerState::Release()
{
	state.Reset();
	delete this;
}

// このクラスの新しいインスタンスの作成
BlendState* BlendState::Create(ID3D11Device* device)
{
	// このクラスのメモリーを確保
	auto result = new BlendState();
	if (result == nullptr) {
		return nullptr;
	}

	// ブレンドステートの作成
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	// 半透明合成を行う
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	auto hr = device->CreateBlendState(&blendDesc, &result->state);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// ネイティブポインターの取得
ID3D11BlendState* BlendState::GetNativePointer()
{
	return state.Get();
}

// BlendFactorの取得
const FLOAT* BlendState::GetBlendFactor()
{
	FLOAT ret[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	return ret;
}

// リソースの解放
void BlendState::Release()
{
	state.Reset();
	delete this;
}
