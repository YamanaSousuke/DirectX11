#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

class D3DApp
{
public:
	// コンストラクタ
	D3DApp(LPCWSTR title, UINT width, UINT height) : title(title), width(width), height(height) { }
	// デストラクタ
	~D3DApp();
	// 全体の初期化
	virtual bool Init();
	// 受信したメッセージに応じた処理
	// LRESULT WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// メッセージループの実行
	int Run();
	// シーンの更新
	virtual void UpdateScene() = 0;
	// シーンの描画
	virtual void DrawScene() = 0;
	// ウィンドウのサイズが変更されたときに呼び出される
	virtual void OnResize();
	// アスペクト比の取得
	float AspectRatio() const;
protected:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// デバイス
	ComPtr<ID3D11Device> device = nullptr;
	// デバイスコンテキスト
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// スワップチェーン
	ComPtr<IDXGISwapChain> swapchain = nullptr;
	// レンダーターゲット
	ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	// 深度ステンシル
	ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	// 深度ステンシルをシェーダーで利用するためのリソースビュー
	ComPtr<ID3D11ShaderResourceView> depthStencilResourceView = nullptr;
	// ビューポート
	D3D11_VIEWPORT viewports[1] = {};
	// ウィンドウハンドル
	HWND hWnd = NULL;
	// 背景色
	const float backgroundColor[4] = { 0.0f, 0.0f, 0.9f, 1.0f };

	// ウィンドウの初期化
	bool InitMainWindow();
	// グラフィックデバイスの初期化
	bool InitGraphicsDevice();
	// GUIの初期化
	bool InitGUI();
private:
	// 機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// 深度ステンシルのフォーマット
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

	// ウィンドウタイトル
	LPCWSTR title = {};
	// 横幅
	UINT width = 0;
	// 縦幅
	UINT height = 0;
};
