#include "D3DApp.h"

using namespace Microsoft::WRL;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// 受信したメッセージに応じた処理
LRESULT WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}
		
	switch (uMsg) {

	// 終了処理
	case WM_DESTROY:	
		PostQuitMessage(0);
		return 0;
	
	default: break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 全体の初期化
bool D3DApp::Init()
{
	if (!InitMainWindow()) {
		return false;
	}

	if (!InitGraphicsDevice()) {
		return false;
	}

	if (!InitGUI()) {
		return false;
	}

	return true;
}

// ウィンドウの初期化
bool D3DApp::InitMainWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// ウィンドウクラスを登録する
	const wchar_t className[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WindowProcedure;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = className;
	if (!RegisterClassEx(&wndClass)) {
		OutputDebugStringA("ウィンドウクラスの登録に失敗\n");
		return false;
	}

	// クライアント領域の指定
	RECT rect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// ウィンドウの作成
	auto hWnd = CreateWindowEx(0, className, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
		OutputDebugStringA("ウィンドウの作成に失敗\n");
		return false;
	}

	this->hWnd = hWnd;
	// ウィンドウの表示
	ShowWindow(hWnd, SW_SHOWNORMAL);
	// クライアント領域の更新
	UpdateWindow(hWnd);

	return true;
}

// グラフィックデバイスの初期化
bool D3DApp::InitGraphicsDevice()
{
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
	// レンダーターゲットとシェーダーリソースとして使用
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Windowed = TRUE;
	// デバイスとスワップチェーンの作成
	const auto hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, crationFlag,
		NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapchain, &device, &featureLevel, &deviceContext);
	if (FAILED(hr)) {
		OutputDebugStringA("デバイスの作成に失敗\n");
		return false;
	}

	// 
	OnResize();

	return true;
}

// GUIの初期化
bool D3DApp::InitGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
	return true;
}

// メッセージループの実行
int D3DApp::Run()
{
	// メッセージループ
	MSG msg = {};
	while (true) {
		// GUIの更新処理
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		UpdateScene();
		DrawScene();

		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

// ウィンドウのサイズが変更されたときに呼び出される
void D3DApp::OnResize()
{
	// リソースのリセット
	renderTargetView.Reset();
	depthStencilView.Reset();
	depthStencilResourceView.Reset();
	
	// スワップチェーンからバックバッファーの取得
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	auto hr = swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputDebugStringA("バックバッファーの取得に失敗\n");
	}

	// バッファーのサイズの変更
	hr = swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(hr)) {
		OutputDebugStringA("バッファーのサイズの変更に失敗\n");
	}

	// レンダーターゲットビューの作成
	hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, renderTargetView.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugStringA("レンダーターゲットビューの作成に失敗\n");
	}
	backBuffer.Reset();

	// テクスチャとシェーダーリソースビューのフォーマットを設定
	DXGI_FORMAT textureFormat = depthStencilFormat;
	DXGI_FORMAT resourceFormat = depthStencilFormat;
	switch (depthStencilFormat)
	{
	case DXGI_FORMAT_D16_UNORM:
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		resourceFormat = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	}

	// 深度ステンシルの作成
	ComPtr<ID3D11Texture2D> depthStencil = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = textureFormat;
	depthStencilDesc.SampleDesc = { 1, 0 };
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
	if (FAILED(hr)) {
		OutputDebugStringA("深度ステンシルの作成に失敗\n");
	}

	// 深度ステンシルビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = depthStencilFormat;
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	}
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, depthStencilView.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugStringA("深度ステンシルビューの作成に失敗\n");
	}

	// 深度ステンシルリソースビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc = {};
	depthStencilResourceViewDesc.Format = resourceFormat;
	if (depthStencilDesc.SampleDesc.Count > 0) {
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
	}
	else {
		depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		depthStencilResourceViewDesc.Texture2D.MostDetailedMip = 0;
		depthStencilResourceViewDesc.Texture2D.MipLevels = 1;
	}
	hr = device->CreateShaderResourceView(depthStencil.Get(), &depthStencilResourceViewDesc, depthStencilResourceView.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugStringA("深度ステンシルリソースビューの作成に失敗\n");
	}

	// ビューポート
	viewports[0].Width = static_cast<FLOAT>(width);
	viewports[0].Height = static_cast<FLOAT>(height);
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;
}

// アスペクト比の取得
float D3DApp::AspectRatio() const
{
	return static_cast<float>(width) / height;
}

// デストラクター
D3DApp::~D3DApp()
{
	// GUIの開放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
