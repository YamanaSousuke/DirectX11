#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

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
	// 画面をクリアするときに使用するカラー
	const FLOAT clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	// ビューポート
	D3D11_VIEWPORT viewports[1] = {};

	// エフェクト
	Effect effect = {};


	// ウィンドウの作成
	bool InitWindow();
	// グラフィックデバイスの作成
	bool InitGraphicsDevice();
	// GUIの初期化
	bool InitGUI();
};

// 頂点シェーダー
class VertexShader
{
private:
	//  リソース
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader = nullptr;
public:
	// このクラスの新しいインスタンスの作成
	VertexShader(ID3D11Device* device);
	// バイトコードの取得
	const BYTE* GetBytecode();
	// バイトコードのサイズの取得
	SIZE_T GetBytecodeLength();
	// ネイティブポインターの取得
	ID3D11VertexShader* GetNativePointer();
	// リソースの解放
	void Release();
};

// ジオメトリシェーダー
class GeometryShader
{
private:
	//  リソース
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader = nullptr;
public:
	// このクラスの新しいインスタンスの作成
	GeometryShader(ID3D11Device* device);
	// ネイティブポインターの取得
	ID3D11GeometryShader* GetNativePointer();
	// リソースの解放
	void Release();
};

// ピクセルシェーダー
class PixelShader {
private:
	//  リソース
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader = nullptr;
public:
	// このクラスの新しいインスタンスの作成
	PixelShader(ID3D11Device* device);
	// ネイティブポインターの取得
	ID3D11PixelShader* GetNativePointer();
	// リソースの解放
	void Release();
};

// インプットレイアウト
class InputLayout
{
	// リソース
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;
public:
	// このクラスの新しいインスタンスの作成
	InputLayout(ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC* descs, UINT numElements,
		const void* shaderBytecode, SIZE_T bytecodeLength);
	// ネイティブポインターの取得
	ID3D11InputLayout* GetNativePointer();
	// リソースの解放
	void Release();
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