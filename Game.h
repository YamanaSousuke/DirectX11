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
	// ウィンドウのタイトル
	LPCWSTR title = L"Title";
	// ウィンドウの幅
	int width = 640;
	// ウィンドウの高さ
	int height = 480;
	// ウィンドウハンドル
	HWND hWnd = NULL;
	// デバイス
	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	// デバイスコンテキスト
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// 機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain = nullptr;
	// レンダーターゲット
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView[1];
	// 深度ステンシル
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	// 深度ステンシルをシェーダーで利用するためのリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthStencilResourceView = nullptr;
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

// 2Dテクスチャー
class Texture2D
{
	// リソース
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
	// サンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState = nullptr;
	// シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;

public:
	// このクラスの新しいインスタンスの作成
	Texture2D(ID3D11Device* device, UINT width, UINT height,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, bool mipChain = true);
	// バッファーにデータを設定する
	void SetData(const void* data);
	// ネイティブポインターの取得
	ID3D11Texture2D* GetNativePointer();
	ID3D11SamplerState* GetSapmlerState();
	ID3D11ShaderResourceView* GetShaderResourceView();
	// リソースの解放
	void Release();
};

// ラスタライザステート
class RasterizerState
{
	// リソース
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> state = nullptr;
public:
	// このクラスの新しいインスタンスの作成
	static RasterizerState* Create(ID3D11Device* device);
	// ネイティブポインターの取得
	ID3D11RasterizerState* GetNativePointer();
	// リソースの解放
	void Release();
};

// ブレンドステート
class BlendState
{
	// リソース
	Microsoft::WRL::ComPtr<ID3D11BlendState> state = nullptr;
public:
	// このクラスの新しいインスタンスの作成
	static BlendState* Create(ID3D11Device* device);
	// ネイティブポインターの取得
	ID3D11BlendState* GetNativePointer();
	// BlendFactorの取得
	const FLOAT* GetBlendFactor();
	// リソースの解放
	void Release();
};