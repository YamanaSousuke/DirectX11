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

	HRESULT hr = S_OK;

	// デバイス作成時のオプションフラグ
	UINT crationFlag = 0;
#if defined(_DEBUG)
	crationFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 作成する機能レベル
	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// デバイスの作成
	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, crationFlag, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &device, &featureLevel, &deviceContext);
	if (FAILED(hr))
	{
		OutputDebugStringA("デバイスの作成に失敗\n");
		return -1;
	}

	// メッセージループ
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	device.Reset();
	deviceContext.Reset();
	return 0;
}