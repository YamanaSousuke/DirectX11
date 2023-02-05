#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "FBXMeshFile.h"
#include "GameObject.h"

#include "Lightings.h"
#include "Effect.h"

// アプリケーション全体を表す
class Game
{
public:
	// アプリケーションの初期化
	void Initialize(LPCWSTR title, int width, int height);
	// メッセージループの実行
	int Run();

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ウィンドウのタイトル
	LPCWSTR title = L"Title";
	// ウィンドウの幅
	int width = 640;
	// ウィンドウの高さ
	int height = 480;
	// ウィンドウハンドル
	HWND hWnd = NULL;
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

	// エフェクト
	Effect effect = {};
	// FBX
	FbxMeshFile fbxMeshfile = {};
	// 家
	GameObject house;


	// フォグについての設定
	bool fogEnable = false;
	DirectX::XMFLOAT3 fogColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float fogStart = 0.0f;
	float fogEnd = 0.0f;

	// ウィンドウの作成
	bool InitWindow();
	// グラフィックデバイスの作成
	bool InitGraphicsDevice();
	// GUIの初期化
	bool InitGUI();
};