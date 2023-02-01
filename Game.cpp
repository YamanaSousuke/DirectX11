#include <d3dcompiler.h>
#include "Game.h"
#include "Geometry.h"
#include "Vertex.h"
#include "GameObject.h"
#include "DDSTextureLoader.h"
#include "ModelManager.h"
#include <DirectXColors.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

using namespace Microsoft::WRL;
using namespace DirectX;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
// ��M�������b�Z�[�W�ɉ���������
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}

	switch (uMsg) {
	case WM_DESTROY:
		// �I������
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// �A�v���P�[�V�����̏�����
void Game::Initialize(LPCWSTR title, int width, int height)
{
	this->title = title;
	this->width = width;
	this->height = height;
}

// �E�B���h�E�̍쐬
bool Game::InitWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	// �E�B���h�E�N���X��o�^����
	const wchar_t className[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = className;
	if (!RegisterClassEx(&wndClass))
	{
		OutputDebugStringA("�E�B���h�E�N���X�̓o�^�Ɏ��s\n");
		return false;
	}

	// �N���C�A���g�̈�̎w��
	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// �E�B���h�E�̍쐬
	auto hWnd = CreateWindowEx(0, className, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		OutputDebugStringA("�E�B���h�E�̍쐬�Ɏ��s\n");
		return false;
	}

	this->hWnd = hWnd;

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, SW_SHOWNORMAL);
	// �N���C�A���g�̈�̍X�V
	UpdateWindow(hWnd);

	return true;
}

// �O���t�B�b�N�f�o�C�X�̍쐬
bool Game::InitGraphicsDevice()
{
	HRESULT hr = S_OK;

	// �f�o�C�X�쐬���̃I�v�V�����t���O
	UINT crationFlag = 0;
#if defined(_DEBUG)
	crationFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// �X���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc = { 1, 0 };
	// �����_�[�^�[�Q�b�g�ƃV�F�[�_�[���\�[�X�Ƃ��Ďg�p
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Windowed = TRUE;
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, crationFlag,
		NULL, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapchain, &device, &featureLevel, &deviceContext);
	if (FAILED(hr)) {
		OutputDebugStringA("�f�o�C�X�̍쐬�Ɏ��s\n");
		return false;
	}

	// �X���b�v�`�F�[������o�b�N�o�b�t�@�[�̎擾
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputDebugStringA("�o�b�N�o�b�t�@�[�̎擾�Ɏ��s\n");
		return false;
	}

	// �����_�[�^�[�Q�b�g�r���[�̍쐬
	hr = device->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView[0]);
	if (FAILED(hr)) {
		OutputDebugStringA("�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s\n");
		return false;
	}
	backBuffer.Reset();

	// �e�N�X�`���ƃV�F�[�_�[���\�[�X�r���[�̃t�H�[�}�b�g��ݒ�
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

	// �[�x�X�e���V���̍쐬
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
		OutputDebugStringA("�[�x�X�e���V���̍쐬�Ɏ��s\n");
		return false;
	}

	// �[�x�X�e���V���r���[�̍쐬
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
		OutputDebugStringA("�[�x�X�e���V���r���[�̍쐬�Ɏ��s\n");
		return false;
	}

	// �[�x�X�e���V�����\�[�X�r���[�̍쐬
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
		OutputDebugStringA("�[�x�X�e���V�����\�[�X�r���[�̍쐬�Ɏ��s\n");
		return false;
	}
	depthStencil.Reset();

	// �r���[�|�[�g
	viewports[0].Width = static_cast<FLOAT>(width);
	viewports[0].Height = static_cast<FLOAT>(height);
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;

	return true;
}

// GUI�̏�����
bool Game::InitGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
	return true;
}

// ���b�Z�[�W���[�v�̎��s
int Game::Run()
{
	// �E�B���h�E�̍쐬
	if (!InitWindow()) {
		return -1;
	}

	// �O���t�B�b�N�f�o�C�X�̍쐬
	if (!InitGraphicsDevice()) {
		return -1;
	}

	// GUI�̏�����
	if (!InitGUI()) {
		return -1;
	}

	// Model model(6);

	HRESULT hr = S_OK;

	// �e�N�X�`���ƃ}�e���A��
	ComPtr<ID3D11ShaderResourceView> texture = nullptr;
	Material material = {};
	material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �ؔ��̕`��̏���
	auto box = new GameObject();
	DirectX::CreateDDSTextureFromFile(device.Get(), L"Texture/Wood.dds", nullptr, texture.GetAddressOf());
	box->SetBuffer(device.Get(), deviceContext.Get(), Geometry::CreateBox<VertexPositionNormalTexture>());
	box->SetTexture(texture.Get());
	box->GetTransform().SetPosition(0.0f, -1.0f, 0.0f);
	box->SetMaterial(material);

	if (!effect.InitAll(device.Get())) {
		OutputDebugString(L"�G�t�F�N�g�̏������Ɏ��s");
		return -1;
	}

	// �T���v���[�̍쐬
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
		OutputDebugString(L"�T���v���[�̍쐬�Ɏ��s\n");
	}

	float time = 0.0f;
	// ���b�Z�[�W���[�v
	MSG msg = {};
	while (true) {
		time += 0.01666f;
		// GUI�̍X�V����
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// �t�H�O�ɂ��Ă̐���
		ImGui::Begin("Fog");
		ImGui::Checkbox("Enable Fog", &fogEnable);
		ImGui::ColorEdit3("Color", &fogColor.x);
		ImGui::DragFloat("Fog Start", &fogStart, 0.05f, 0.0f, 0.0f, "%.1f");
		ImGui::DragFloat("Fog End", &fogEnd, 0.05f, 0.0f, 0.0f, "%.1f");
		ImGui::End();

		// �r���[�s��
		XMVECTOR eyePosition = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
		XMVECTOR focusPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX view = XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

		effect.SetViewMatrix(eyePosition, focusPosition, upDirection);

		// �v���W�F�N�V�����s��
		auto fovAngleY = 60.0f;
		auto aspectRatio = (float)(width) / (float)(height);
		auto nearZ = 0.3f;
		auto farZ = 1000.0f;
		XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleY), aspectRatio, nearZ, farZ);
		effect.SetProjectionMatrix(XMConvertToRadians(fovAngleY), aspectRatio, nearZ, farZ);

		// ���C�g
		DirectionalLight directionalLight[4] = {};
		directionalLight[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLight[0].direction = XMFLOAT4(1.0f, 2.0f, -1.0f, 0.0f);
		directionalLight[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		directionalLight[1].direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
		for (UINT i = 0; i < ARRAYSIZE(directionalLight); i++) {
			effect.SetDirectionalLight(i, directionalLight[i]);
		}
		// ���_
		XMFLOAT3 eyePositionFloat3 = XMFLOAT3(0.0f, 0.0f, -10.0f);
		effect.SetEyePosition(eyePositionFloat3);

		// �t�H�O�ɂ��Ă̐ݒ�
		effect.SetFogColor(fogColor);
		effect.SetFogState(fogEnable);
		effect.SetFogStart(fogStart);
		effect.SetFogRange(abs(fogEnd - fogStart));

		// �����_�[�^�[�Q�b�g��ݒ�
		deviceContext->OMSetRenderTargets(_countof(renderTargetView), renderTargetView->GetAddressOf(), depthStencilView.Get());
		// ��ʂ̃N���A
		deviceContext->ClearRenderTargetView(renderTargetView[0].Get(), reinterpret_cast<const float*>(&Colors::Silver));
		deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		deviceContext->RSSetViewports(_countof(viewports), viewports);

		// �f�t�H���g�̕`��
		effect.RenderDefault(deviceContext.Get());
		ID3D11SamplerState* samplerStates[1] = { samplerState.Get() };
		deviceContext->PSSetSamplers(0, 1, samplerStates);

		// �`��
		effect.Apply(deviceContext.Get());
		box->Draw(deviceContext.Get(), effect);

		// sphere->Draw(deviceContext.Get());
		// ground->Draw(deviceContext.Get());

		// GUI�̕`��
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		// �\��
		swapchain->Present(1, 0);

		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W���擾
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// ���\�[�X�̉��
	box->Release();

	// GUI�̊J��
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return 0;
}