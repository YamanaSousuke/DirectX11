#include <d3d11.h>
#include "Game.h"
#include <wrl/client.h>

// 受信したメッセージに応じた処理
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		// 終了処理
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// アプリケーションの初期化
void Game::Initialize(LPCWSTR title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;
}

// ウィンドウの作成
bool Game::InitWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// ウィンドウクラスを登録する
	const wchar_t className[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = className;
	if (!RegisterClassEx(&wndClass))
	{
		OutputDebugStringA("ウィンドウクラスの登録に失敗\n");
		return false;
	}

	// クライアント領域の指定
	RECT rect = { 0, 0, 640, 480 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0, className, L"Title", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		OutputDebugStringA("ウィンドウの作成に失敗\n");
		return false;
	}

	// ウィンドウの表示
	ShowWindow(hWnd, SW_SHOWNORMAL);
	// クライアント領域の更新
	UpdateWindow(hWnd);

	return true;
}

// メッセージループの実行
int Game::Run()
{
	// ウィンドウの作成に失敗
	if (!InitWindow()) {
		return-1;
	}

	// デバイス
	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	// デバイスコンテキスト
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// 機能レベル
	D3D_FEATURE_LEVEL featureLevel = {};
	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain = nullptr;
	// レンダーターゲット
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;

	HRESULT hr = S_OK;

	// デバイス作成時のオプションフラグ
	UINT crationFlag = 0;
#if defined(_DEBUG)
	crationFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// スワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Windowed = TRUE;
	// デバイスとスワップチェーンの作成
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, crationFlag, 
		NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapchain, &device, &featureLevel, &deviceContext);
	if (FAILED(hr)) {
		OutputDebugStringA("デバイスの作成に失敗\n");
		return -1;
	}

	// メッセージループ
	MSG msg = {};
	while (true) {
		// 表示
		swapchain->Present(1, 0);


		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// リソースの解放
	device.Reset();
	deviceContext.Reset();
	swapchain.Reset();

	return 0;
}