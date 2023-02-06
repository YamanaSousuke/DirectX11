#pragma once

#include <d3d11.h>
#include <wrl/client.h>

// サンプラー、ブレンド、ラスタライザステートを扱う
class RenderState
{
public:
	// 初期化
	static bool InitAll(ID3D11Device* device);

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// 線形補間のサンプリング
	static ComPtr<ID3D11SamplerState> linerSamplerState;
};
