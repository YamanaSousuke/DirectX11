#include "Game.h"

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
	if (!RegisterClassEx(&wndClass)) {
		OutputDebugStringA("�E�B���h�E�N���X�̓o�^�Ɏ��s\n");
		return false;
	}

	// �N���C�A���g�̈�̎w��
	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// �E�B���h�E�̍쐬
	auto hWnd = CreateWindowEx(0, className, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top), NULL, NULL, hInstance, NULL);
	if (hWnd == NULL) {
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
	ComPtr<ID3D11Texture2D> depthStencil = nullptr;
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

	// FBX�t�@�C���ǂݍ��݋@�\�̏�����
	fbxMeshfile.Init(device.Get());

	// �T���v���[�A�u�����h�A���X�^���C�U�X�e�[�g�̏�����
	if (!RenderState::InitAll(device.Get())) {
		return -1;
	}

	// �V�F�[�_�[�ƒ萔�o�b�t�@�[�̏�����
	if (!effect.InitAll(device.Get())) {
		return -1;
	}

	// FBX���f���̓ǂݍ���
	auto model = fbxMeshfile.Load("Models/House1/House1.fbx", deviceContext.Get());
	GameObject house(model);
	house.GetTransform().SetScale(0.05f, 0.05f, 0.05f);
	house.GetTransform().SetPosition(-1.0f, 0.0f, 0.0f);
	house.GetTransform().SetRotation(0.0f, XM_PIDIV2, 0.0f);

	// ���b�Z�[�W���[�v
	MSG msg = {};
	while (true) {
		// GUI�̍X�V����
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// �t�H�O�ɂ��Ă̐���
		ImGui::Begin("Fog");
		ImGui::Checkbox("Enable Fog", &fogEnable);
		ImGui::ColorEdit3("Color", &fogColor.x);
		ImGui::DragFloat("Fog Start", &fogStart, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Fog End", &fogEnd, 0.05f, 0.0f, 0.0f, "%.2f");
		ImGui::End();

		// ���f���ɂ��Ă̐���
		ImGui::Begin("Models");
		ImGui::Text("modelName : %s", house.GetModel().GetModelName());

		static auto position = house.GetTransform().GetPosition();
		// static auto rotation = XMFLOAT3(0.0f, XM_PIDIV2, 0.0f);
		static auto scale = house.GetTransform().GetScale();

		// �g�����X�t�H�[��
		if (ImGui::TreeNode("Transform")) {
			ImGui::Text("Position");
			ImGui::DragFloat3("##position", &position.x, 0.05f, 0.0f, 0.0f, "%.2f");
			// ImGui::Text("Rotation");
			// ImGui::DragFloat3("##rotation", &rotation.x, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::Text("Scale");
			ImGui::DragFloat3("##scale", &scale.x, 0.05f, 0.0f, 0.0f, "%.2f");
			ImGui::TreePop();
		}

		house.GetTransform().SetPosition(position);
		// house.GetTransform().SetRotation(XMConvertToRadians(rotation.x), XMConvertToRadians(rotation.y), XMConvertToRadians(rotation.z));
		house.GetTransform().SetScale(scale);

		auto& meshList = house.GetModel().GetMeshData();
		// �}�e���A���ɂ��Ă̐���
		if (ImGui::TreeNode("Material")) {
			 
			for (auto& mesh : meshList) {
				ImGui::Text("material : %s", mesh.materialName.c_str());
			}
			ImGui::TreePop();
		}
		// �e�N�X�`���[�ɂ��Ă̐���
		if (ImGui::TreeNode("Texture")) {
			for (auto& mesh : meshList) {
				if (!mesh.textureName.empty()) {
					ImGui::Text("texture : %s", mesh.textureName.c_str());
				}
			}
			ImGui::TreePop();
		}

		static float smooth = 0.0f;
		static float metallic = 0.0f;
		ImGui::Text("smooth");
		ImGui::SliderFloat("##smooth", &smooth, 0.0f, 1.0f, "%.2f");
		ImGui::Text("metallic");
		ImGui::SliderFloat("##metallic", &metallic, 0.0f, 1.0f, "%.2f");
		for (auto& mesh : meshList) {
			mesh.material.smooth = smooth;
			mesh.material.metallic = metallic;
		}
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
		directionalLight[0].diffuse = XMFLOAT4(1.0f, 0.84f, 0.0f, 1.0f);
		directionalLight[0].ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		directionalLight[0].direction = XMFLOAT4(-1.0f, -1.0f, 2.0f, 0.0f);
		directionalLight[1].diffuse = XMFLOAT4(1.0f, 0.84f, 0.0f, 1.0f);
		directionalLight[1].ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		directionalLight[1].direction = XMFLOAT4(-1.0f, -1.0f, 0.0f, 0.0f);
		directionalLight[2].diffuse = XMFLOAT4(1.0f, 0.84f, 0.0f, 1.0f);
		directionalLight[2].ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		directionalLight[2].direction = XMFLOAT4(-1.0f, -1.0f, -2.0f, 0.0f);
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
		deviceContext->OMSetRenderTargets(ARRAYSIZE(renderTargetView), renderTargetView->GetAddressOf(), depthStencilView.Get());
		// ��ʂ̃N���A
		auto backgroundColor = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		deviceContext->ClearRenderTargetView(renderTargetView[0].Get(), reinterpret_cast<const float*>(&backgroundColor));
		deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		deviceContext->RSSetViewports(ARRAYSIZE(viewports), viewports);

		// �`��
		effect.RenderDefault(deviceContext.Get());
		effect.Apply(deviceContext.Get());
		house.Draw(deviceContext.Get(), effect);

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

	// GUI�̊J��
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	return 0;
}