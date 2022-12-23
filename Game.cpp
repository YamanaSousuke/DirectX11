#include <d3dcompiler.h>
#include "Game.h"

using namespace Microsoft::WRL;
using namespace DirectX;

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

// メッセージループの実行
int Game::Run()
{
	// ウィンドウの作成
	if (!InitWindow()) {
		return-1;
	}

	// グラフィックデバイスの作成
	if (!InitGraphicsDevice()) {
		return-1;
	}

	HRESULT hr = S_OK;

	// 頂点データの配列
	VertexPositionNormal vertices[] = {
		// Front
		{ {-1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {-1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
		{ { 1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
		{ {-1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
		// Back
		{ {-1.0f, 1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f } },
		{ { 1.0f, 1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f } },
		{ {-1.0f,-1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f } },
		{ { 1.0f,-1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f } },
		{ {-1.0f,-1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f } },
		{ { 1.0f, 1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f } },
		// Right
		{ { 1.0f, 1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { 1.0f,-1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { 1.0f,-1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { 1.0f,-1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
		// Left
		{ {-1.0f, 1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f } },
		{ {-1.0f,-1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f } },
		{ {-1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } },
		{ {-1.0f,-1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } },
		{ {-1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f } },
		{ {-1.0f,-1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f } },
		// UP
		{ {-1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ {-1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { 1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ {-1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
		// DOWN
		{ {-1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } },
		{ {-1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f } },
		{ { 1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } },
		{ { 1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f } },
		{ { 1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f } },
		{ {-1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f } },
	};

	// 頂点バッファーの作成
	auto vertexBuffer = VertexBuffer::Create(device.Get(), sizeof(vertices));
	if (vertexBuffer == nullptr) {
		OutputDebugStringA("頂点バッファーの作成に失敗\n");
		return -1;
	}
	// 頂点バッファーにデータを転送
	vertexBuffer->SetData(vertices);

	// インデックスデータ
	UINT32 indices[] = {
		 0,  1,  2,  3,  4,  5,
		 6,  7,  8,  9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35,
	};

	// インデックスバッファーの作成
	auto indexBuffer = IndexBuffer::Create(device.Get(), _countof(indices));
	if (indexBuffer == nullptr) {
		OutputDebugStringA("インデックスバッファーの作成に失敗\n");
		return -1;
	}
	// インデックスバッファーにデータを転送
	indexBuffer->SetData(indices);


	// 定数バッファーでシェーダーに毎フレーム送るデータ
	struct SceneParameter {
		XMFLOAT4X4 world;					// ワールド行列
		XMFLOAT4X4 view;					// ビュー行列
		XMFLOAT4X4 projection;				// プロジェクション行列
		XMFLOAT4X4 worldViewProjection;		// WVP行列
		float time = 0.0f;				    // 時間
		float alpha = 1.0f;					// アルファ値
	};
	
	// 定数バッファーの作成
	auto constantBuffer = ConstantBuffer::Create(device.Get(), sizeof(SceneParameter));
	if (constantBuffer == nullptr) {
		OutputDebugStringA("定数バッファーの作成に失敗\n");
		return -1;
	}

	// 頂点シェーダーの作成
	auto vertexShader = VertexShader::Create(device.Get());
	if (vertexBuffer == nullptr) {
		OutputDebugStringA("頂点シェーダーの作成に失敗\n");
		return -1;
	}

	// ジオメトリシェーダーの作成
	auto geometryShader = GeometryShader::Create(device.Get());
	if (geometryShader == nullptr) {
		OutputDebugStringA("ジオメトリシェーダーの作成に失敗\n");
		return -1;
	}

	// ピクセルシェーダーの作成
	auto pixelShader = PixelShader::Create(device.Get());
	if (pixelShader == nullptr) {
		OutputDebugStringA("ピクセルシェーダーの作成に失敗\n");
		return -1;
	}

	// 入力レイアウトの作成
	auto inputLayout = InputLayout::Create(device.Get(), vertices->GetInputElementDescs(), vertices->GetInputElementDescsLength(),
		vertexShader->GetBytecode(), vertexShader->GetBytecodeLength());
	if (inputLayout == nullptr) {
		OutputDebugStringA("入力レイアウトの作成に失敗\n");
		return -1;
	}

	// ラスタライザステートの作成
	auto rasterizerState = RasterizerState::Create(device.Get());
	if (rasterizerState == nullptr) {
		OutputDebugStringA("ラスタライザステートの作成に失敗\n");
		return -1;
	}

	// ブレンドステートの作成
	auto blendState = BlendState::Create(device.Get());
	if (blendState == nullptr) {
		OutputDebugStringA("ブレンドステートの作成に失敗\n");
		return -1;
	}

	float time = 0.0f;

	// メッセージループ
	MSG msg = {};
	while (true) {
		time += 0.01666f;

		// ワールド行列
		XMMATRIX world = XMMatrixIdentity();
		// 回転
		XMVECTOR axis = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
		world *= XMMatrixRotationAxis(axis, time);

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
		XMStoreFloat4x4(&matricesPerFrame.world, XMMatrixTranspose(world));
		XMStoreFloat4x4(&matricesPerFrame.view, XMMatrixTranspose(view));
		XMStoreFloat4x4(&matricesPerFrame.projection, XMMatrixTranspose(projection));
		XMStoreFloat4x4(&matricesPerFrame.worldViewProjection, XMMatrixTranspose(world * view * projection));
		matricesPerFrame.time = time;
		if (matricesPerFrame.alpha > 0.0f)
		{
			matricesPerFrame.alpha -= time;
		}
		// 定数バッファの更新
		constantBuffer->SetData(&matricesPerFrame);

		// レンダーターゲットを設定
		deviceContext->OMSetRenderTargets(_countof(renderTargetView), renderTargetView->GetAddressOf(), depthStencilView.Get());
		// ブレンドステートを設定
		deviceContext->OMSetBlendState(blendState->GetNativePointer(), blendState->GetBlendFactor(), 0xffffffff);

		// 画面のクリア
		deviceContext->ClearRenderTargetView(renderTargetView[0].Get(), clearColor);
		deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// ビューポートの設定
		deviceContext->RSSetViewports(_countof(viewports), viewports);

		// 頂点バッファーを設定
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer->GetNativePointer() };
		UINT strides[1] = { sizeof(VertexPositionNormal) };
		UINT offsets[1] = { 0 };
		deviceContext->IASetVertexBuffers(0, _countof(vertexBuffers), vertexBuffers, strides, offsets);

		// シェーダーを設定
		deviceContext->VSSetShader(vertexShader->GetNativePointer(), NULL, 0);
		deviceContext->GSSetShader(geometryShader->GetNativePointer(), NULL, 0);
		deviceContext->PSSetShader(pixelShader->GetNativePointer(), NULL, 0);

		// ジオメトリシェーダーに定数バッファーを設定
		ID3D11Buffer* constantBuffers[1] = { constantBuffer->GetNativePointer() };
		deviceContext->GSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);

		// インプットレイアウトの設定
		deviceContext->IASetInputLayout(inputLayout->GetNativePointer());
		// ラスタライザステートの設定
		deviceContext->RSSetState(rasterizerState->GetNativePointer());

		// トライアングル
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// インデックスバッファーの設定
		deviceContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), DXGI_FORMAT_R32_UINT, 0);
		// 描画
		deviceContext->DrawIndexed(_countof(indices), 0, 0);

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
	indexBuffer->Release();
	vertexBuffer->Release();
	constantBuffer->Release();
	vertexShader->Release();
	geometryShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	rasterizerState->Release();
	blendState->Release();

	Release();
	return 0;
}

// リソースの解放
void Game::Release()
{
	depthStencilResourceView.Reset();
	depthStencilView.Reset();
	renderTargetView->Reset();
	swapchain.Reset();
	deviceContext.Reset();
	device.Reset();
}