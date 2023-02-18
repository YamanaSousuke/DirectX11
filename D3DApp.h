#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

class D3DApp
{
public:
	// コンストラクタ
	D3DApp(LPCWSTR title, UINT width, UINT height)
		: title(title), width(width), height(height) { }
	// 初期化
	bool Init();
	// ウィンドウの初期化
	bool InitMainWindow();
	// グラフィックデバイスの作成
	bool InitGraphicsDevice();
	// ウィンドウハンドル
	HWND hWnd = NULL;

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// デバイス
	ComPtr<ID3D11Device> device = nullptr;
	// デバイスコンテキスト
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// 機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// スワップチェーン
	ComPtr<IDXGISwapChain> swapchain = nullptr;
	// レンダーターゲット
	ComPtr<ID3D11RenderTargetView> renderTargetView[1];
	// 深度ステンシル
	ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	// 深度ステンシルをシェーダーで利用するためのリソースビュー
	ComPtr<ID3D11ShaderResourceView> depthStencilResourceView = nullptr;
	// 深度ステンシルのフォーマット
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	// ビューポート
	D3D11_VIEWPORT viewports[1] = {};
private:
	// ウィンドウハンドル
	HINSTANCE hInstance = {};
	
	// ウィンドウタイトル
	LPCWSTR title = {};
	// 横幅
	UINT width = 0;
	// 縦幅
	UINT height = 0;
};
