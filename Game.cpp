#include <d3dcompiler.h>
#include "Game.h"
#include "Geometry.h"
#include "Vertex.h"
#include "GameObject.h"
#include "DDSTextureLoader.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

using namespace Microsoft::WRL;
using namespace DirectX;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// 受信したメッセージに応じた処理
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}

	switch (uMsg) {
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
	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// ウィンドウの作成
	auto hWnd = CreateWindowEx(0, className, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
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

// グラフィックデバイスの作成
bool Game::InitGraphicsDevice()
{
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
	// レンダーターゲットとシェーダーリソースとして使用
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Windowed = TRUE;
	// デバイスとスワップチェーンの作成
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, crationFlag,
		NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapchain, &device, &featureLevel, &deviceContext);
	if (FAILED(hr)) {
		OutputDebugStringA("デバイスの作成に失敗\n");
		return false;
	}

	// スワップチェーンからバックバッファーの取得
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputDebugStringA("バックバッファーの取得に失敗\n");
		return false;
	}

	// レンダーターゲットビューの作成
	hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView[0]);
	if (FAILED(hr)) {
		OutputDebugStringA("レンダーターゲットビューの作成に失敗\n");
		return false;
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
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = textureFormat;
	depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
	if (FAILED(hr)) {
		OutputDebugStringA("深度ステンシルの作成に失敗\n");
		return false;
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
	hr = device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &depthStencilView);
	if (FAILED(hr)) {
		OutputDebugStringA("深度ステンシルビューの作成に失敗\n");
		return false;
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
	hr = device->CreateShaderResourceView(depthStencil.Get(), &depthStencilResourceViewDesc, &depthStencilResourceView);
	if (FAILED(hr)) {
		OutputDebugStringA("深度ステンシルリソースビューの作成に失敗\n");
		return false;
	}
	depthStencil.Reset();

	// ビューポート
	viewports[0].Width = static_cast<FLOAT>(width);
	viewports[0].Height = static_cast<FLOAT>(height);
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;

	return true;
}

// GUIの初期化
bool Game::InitGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// キーボードによる操作を可能にする
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// // ヘッダーのドラッグのみ可能
	// io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
	return true;
}

// メッセージループの実行
int Game::Run()
{
	// ウィンドウの作成
	if (!InitWindow()) {
		return -1;
	}

	// グラフィックデバイスの作成
	if (!InitGraphicsDevice()) {
		return -1;
	}

	if (!InitGUI()) {
		return -1;
	}

	HRESULT hr = S_OK;

	// テクスチャとマテリアル
	ComPtr<ID3D11ShaderResourceView> texture = nullptr;
	Material material = {};
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.smooth = 0.0f;
	material.metallic = 0.0f;

	// 木箱の描画の準備
	auto box = new GameObject();
	DirectX::CreateDDSTextureFromFile(device.Get(), L"Texture/Wood.dds", nullptr, texture.GetAddressOf());
	box->SetBuffer(device.Get(), deviceContext.Get(), Geometry::CreateBox<VertexPositionNormalTexture>());
	box->SetTexture(texture.Get());
	box->GetTransform().SetPosition(XMFLOAT3(-2.0f, 0.0f, 0.0f));
	box->SetMaterial(material);

	// 球の描画の準備
	auto sphere = new GameObject();
	DirectX::CreateDDSTextureFromFile(device.Get(), L"Texture/Uranus.dds", nullptr, texture.ReleaseAndGetAddressOf());
	sphere->SetBuffer(device.Get(), deviceContext.Get(), Geometry::CreateSphere<VertexPositionNormalTexture>());
	sphere->SetTexture(texture.Get());
	sphere->GetTransform().SetPosition(XMFLOAT3(2.0f, 0.0f, 0.0f));


	// 地面の描画の準備
	auto ground = new GameObject();
	DirectX::CreateDDSTextureFromFile(device.Get(), L"Texture/Ground.dds", nullptr, texture.ReleaseAndGetAddressOf());
	ground->SetBuffer(device.Get(), deviceContext.Get(), Geometry::CreatePlane<VertexPositionNormalTexture>(XMFLOAT2(20.0f, 20.0f)));
	ground->SetTexture(texture.Get());
	ground->GetTransform().SetPosition(XMFLOAT3(0.0f, -1.0f, 5.0f));

	// モデルの定数バッファーの作成
	auto modelConstantBuffer = new ConstantBuffer(device.Get(), sizeof(ModelParameter));
	if (modelConstantBuffer == nullptr) {
		OutputDebugStringA("モデルの定数バッファーの作成に失敗\n");
		return -1;
	}
	
	// 定数バッファーの作成
	auto constantBuffer = new ConstantBuffer(device.Get(), sizeof(SceneParameter));
	if (constantBuffer == nullptr) {
		OutputDebugStringA("定数バッファーの作成に失敗\n");
		return -1;
	}

	// ライトの定数バッファーの作成
	auto lightConstantBuffer = new ConstantBuffer(device.Get(), sizeof(LightParameter));
	if (lightConstantBuffer == nullptr) {
		OutputDebugStringA("ライトの定数バッファー作成に失敗\n");
		return -1;
	}

	// 頂点シェーダーの作成
	auto vertexShader = new VertexShader(device.Get());
	if (vertexShader == nullptr) {
		OutputDebugStringA("頂点シェーダーの作成に失敗\n");
		return -1;
	}

	// ジオメトリシェーダーの作成
	auto geometryShader = new GeometryShader(device.Get());
	if (geometryShader == nullptr) {
		OutputDebugStringA("ジオメトリシェーダーの作成に失敗\n");
		return -1;
	}

	// ピクセルシェーダーの作成
	auto pixelShader = new PixelShader(device.Get());
	if (pixelShader == nullptr) {
		OutputDebugStringA("ピクセルシェーダーの作成に失敗\n");
		return -1;
	}

	// 入力レイアウトの作成
	auto inputLayout = new InputLayout(device.Get(), VertexPositionNormalTexture::inputLayout,
		ARRAYSIZE(VertexPositionNormalTexture::inputLayout), vertexShader->GetBytecode(), vertexShader->GetBytecodeLength());
	if (inputLayout == nullptr) {
		OutputDebugStringA("入力レイアウトの作成に失敗\n");
		return -1;
	}

	// サンプラーの作成
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	ComPtr<ID3D11SamplerState> samplerState = nullptr;
	hr = device->CreateSamplerState(&samplerDesc, samplerState.GetAddressOf());
	if (FAILED(hr)) {
		OutputDebugString(L"サンプラーの作成に失敗\n");
	}


	float time = 0.0f;

	// メッセージループ
	MSG msg = {};
	while (true) {
		time += 0.01666f;

		// ImGuiの更新処理
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (ImGui::Begin("Debug")) {

		}

		ImGui::End();

		// ビュー行列
		XMVECTOR eyePosition = XMVectorSet(0.0f, 1.0f, -10.0f, 1.0f);
		XMVECTOR focusPosition = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX view = XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
		// プロジェクション行列
		auto fovAngleY = 60.0f;
		auto aspectRatio = (float)(width) / (float)(height);
		auto nearZ = 0.3f;
		auto farZ = 1000.0f;
		XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleY), aspectRatio, nearZ, farZ);

		// 用意した定数バッファの構造体に値を設定する
		SceneParameter matricesPerFrame = {};
		XMStoreFloat4x4(&matricesPerFrame.view, XMMatrixTranspose(view));
		XMStoreFloat4x4(&matricesPerFrame.projection, XMMatrixTranspose(projection));
		constantBuffer->SetData(&matricesPerFrame);

		// ライト
		LightParameter lightParameter = {};
		lightParameter.directionalLight[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		lightParameter.directionalLight[0].direction = XMFLOAT4(1.0f, 4.0f, -2.0f, 0.0f);
		lightParameter.directionalLight[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		lightParameter.directionalLight[1].direction = XMFLOAT4(1.0f, 2.0f, -2.0f, 0.0f);
		lightParameter.eyePosition = XMFLOAT4(0.0f, 1.0f, -10.0f, 1.0f);
		lightParameter.ambient = 0.2f;
		lightConstantBuffer->SetData(&lightParameter);

		// レンダーターゲットを設定
		deviceContext->OMSetRenderTargets(_countof(renderTargetView), renderTargetView->GetAddressOf(), depthStencilView.Get());

		// 画面のクリア
		deviceContext->ClearRenderTargetView(renderTargetView[0].Get(), clearColor);
		deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		deviceContext->RSSetViewports(_countof(viewports), viewports);

		// シェーダーを設定
		deviceContext->VSSetShader(vertexShader->GetNativePointer(), NULL, 0);
		deviceContext->GSSetShader(geometryShader->GetNativePointer(), NULL, 0);
		deviceContext->PSSetShader(pixelShader->GetNativePointer(), NULL, 0);

		// ジオメトリシェーダーに定数バッファーを設定
		ID3D11Buffer* gsConstantBuffers[2] = { constantBuffer->GetNativePointer(), modelConstantBuffer->GetNativePointer() };
		deviceContext->GSSetConstantBuffers(0, ARRAYSIZE(gsConstantBuffers), gsConstantBuffers);

		// ピクセルシェーダーに定数バッファーを設定
		ID3D11Buffer* psConstantBuffers[2] = { lightConstantBuffer->GetNativePointer(), modelConstantBuffer->GetNativePointer() };
		deviceContext->PSSetConstantBuffers(0, ARRAYSIZE(psConstantBuffers), psConstantBuffers);

		deviceContext->IASetInputLayout(inputLayout->GetNativePointer());
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11SamplerState* samplerStates[1] = { samplerState.Get() };
		deviceContext->PSSetSamplers(0, 1, samplerStates);

		// 描画
		box->Draw(deviceContext.Get());
		// sphere->Draw(deviceContext.Get());
		// ground->Draw(deviceContext.Get());


		// GUIの描画
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
	constantBuffer->Release();
	modelConstantBuffer->Release();
	lightConstantBuffer->Release();
	vertexShader->Release();
	geometryShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	box->Release();
	sphere->Release();
	ground->Release();

	// GUIの開放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return 0;
}