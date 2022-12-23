#include <d3dcompiler.h>
#include "Game.h"

using namespace Microsoft::WRL;
using namespace DirectX;

// ��M�������b�Z�[�W�ɉ���������
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
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

// ���b�Z�[�W���[�v�̎��s
int Game::Run()
{
	// �E�B���h�E�̍쐬
	if (!InitWindow()) {
		return-1;
	}

	// �O���t�B�b�N�f�o�C�X�̍쐬
	if (!InitGraphicsDevice()) {
		return-1;
	}

	HRESULT hr = S_OK;

	// ���_�f�[�^�̔z��
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

	// ���_�o�b�t�@�[�̍쐬
	auto vertexBuffer = VertexBuffer::Create(device.Get(), sizeof(vertices));
	if (vertexBuffer == nullptr) {
		OutputDebugStringA("���_�o�b�t�@�[�̍쐬�Ɏ��s\n");
		return -1;
	}
	// ���_�o�b�t�@�[�Ƀf�[�^��]��
	vertexBuffer->SetData(vertices);

	// �C���f�b�N�X�f�[�^
	UINT32 indices[] = {
		 0,  1,  2,  3,  4,  5,
		 6,  7,  8,  9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35,
	};

	// �C���f�b�N�X�o�b�t�@�[�̍쐬
	auto indexBuffer = IndexBuffer::Create(device.Get(), _countof(indices));
	if (indexBuffer == nullptr) {
		OutputDebugStringA("�C���f�b�N�X�o�b�t�@�[�̍쐬�Ɏ��s\n");
		return -1;
	}
	// �C���f�b�N�X�o�b�t�@�[�Ƀf�[�^��]��
	indexBuffer->SetData(indices);


	// �萔�o�b�t�@�[�ŃV�F�[�_�[�ɖ��t���[������f�[�^
	struct SceneParameter {
		XMFLOAT4X4 world;					// ���[���h�s��
		XMFLOAT4X4 view;					// �r���[�s��
		XMFLOAT4X4 projection;				// �v���W�F�N�V�����s��
		XMFLOAT4X4 worldViewProjection;		// WVP�s��
		float time = 0.0f;				    // ����
		float alpha = 1.0f;					// �A���t�@�l
	};
	
	// �萔�o�b�t�@�[�̍쐬
	auto constantBuffer = ConstantBuffer::Create(device.Get(), sizeof(SceneParameter));
	if (constantBuffer == nullptr) {
		OutputDebugStringA("�萔�o�b�t�@�[�̍쐬�Ɏ��s\n");
		return -1;
	}

	// ���_�V�F�[�_�[�̍쐬
	auto vertexShader = VertexShader::Create(device.Get());
	if (vertexBuffer == nullptr) {
		OutputDebugStringA("���_�V�F�[�_�[�̍쐬�Ɏ��s\n");
		return -1;
	}

	// �W�I���g���V�F�[�_�[�̍쐬
	auto geometryShader = GeometryShader::Create(device.Get());
	if (geometryShader == nullptr) {
		OutputDebugStringA("�W�I���g���V�F�[�_�[�̍쐬�Ɏ��s\n");
		return -1;
	}

	// �s�N�Z���V�F�[�_�[�̍쐬
	auto pixelShader = PixelShader::Create(device.Get());
	if (pixelShader == nullptr) {
		OutputDebugStringA("�s�N�Z���V�F�[�_�[�̍쐬�Ɏ��s\n");
		return -1;
	}

	// ���̓��C�A�E�g�̍쐬
	auto inputLayout = InputLayout::Create(device.Get(), vertices->GetInputElementDescs(), vertices->GetInputElementDescsLength(),
		vertexShader->GetBytecode(), vertexShader->GetBytecodeLength());
	if (inputLayout == nullptr) {
		OutputDebugStringA("���̓��C�A�E�g�̍쐬�Ɏ��s\n");
		return -1;
	}

	// ���X�^���C�U�X�e�[�g�̍쐬
	auto rasterizerState = RasterizerState::Create(device.Get());
	if (rasterizerState == nullptr) {
		OutputDebugStringA("���X�^���C�U�X�e�[�g�̍쐬�Ɏ��s\n");
		return -1;
	}

	// �u�����h�X�e�[�g�̍쐬
	auto blendState = BlendState::Create(device.Get());
	if (blendState == nullptr) {
		OutputDebugStringA("�u�����h�X�e�[�g�̍쐬�Ɏ��s\n");
		return -1;
	}

	float time = 0.0f;

	// ���b�Z�[�W���[�v
	MSG msg = {};
	while (true) {
		time += 0.01666f;

		// ���[���h�s��
		XMMATRIX world = XMMatrixIdentity();
		// ��]
		XMVECTOR axis = XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f);
		world *= XMMatrixRotationAxis(axis, time);

		// �r���[�s��
		XMVECTOR eyePosition = XMVectorSet(0.0f, 1.0f, -10.0f, 1.0f);
		XMVECTOR focusPosition = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX view = XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
		// �v���W�F�N�V�����s��
		auto fovAngleY = 60.0f;
		auto aspectRatio = (float)(width) / (float)(height);
		auto nearZ = 0.3f;
		auto farZ = 1000.0f;
		XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleY), aspectRatio, nearZ, farZ);

		// �p�ӂ����萔�o�b�t�@�̍\���̂ɒl��ݒ肷��
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
		// �萔�o�b�t�@�̍X�V
		constantBuffer->SetData(&matricesPerFrame);

		// �����_�[�^�[�Q�b�g��ݒ�
		deviceContext->OMSetRenderTargets(_countof(renderTargetView), renderTargetView->GetAddressOf(), depthStencilView.Get());
		// �u�����h�X�e�[�g��ݒ�
		deviceContext->OMSetBlendState(blendState->GetNativePointer(), blendState->GetBlendFactor(), 0xffffffff);

		// ��ʂ̃N���A
		deviceContext->ClearRenderTargetView(renderTargetView[0].Get(), clearColor);
		deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// �r���[�|�[�g�̐ݒ�
		deviceContext->RSSetViewports(_countof(viewports), viewports);

		// ���_�o�b�t�@�[��ݒ�
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer->GetNativePointer() };
		UINT strides[1] = { sizeof(VertexPositionNormal) };
		UINT offsets[1] = { 0 };
		deviceContext->IASetVertexBuffers(0, _countof(vertexBuffers), vertexBuffers, strides, offsets);

		// �V�F�[�_�[��ݒ�
		deviceContext->VSSetShader(vertexShader->GetNativePointer(), NULL, 0);
		deviceContext->GSSetShader(geometryShader->GetNativePointer(), NULL, 0);
		deviceContext->PSSetShader(pixelShader->GetNativePointer(), NULL, 0);

		// �W�I���g���V�F�[�_�[�ɒ萔�o�b�t�@�[��ݒ�
		ID3D11Buffer* constantBuffers[1] = { constantBuffer->GetNativePointer() };
		deviceContext->GSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);

		// �C���v�b�g���C�A�E�g�̐ݒ�
		deviceContext->IASetInputLayout(inputLayout->GetNativePointer());
		// ���X�^���C�U�X�e�[�g�̐ݒ�
		deviceContext->RSSetState(rasterizerState->GetNativePointer());

		// �g���C�A���O��
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// �C���f�b�N�X�o�b�t�@�[�̐ݒ�
		deviceContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), DXGI_FORMAT_R32_UINT, 0);
		// �`��
		deviceContext->DrawIndexed(_countof(indices), 0, 0);

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

// ���\�[�X�̉��
void Game::Release()
{
	depthStencilResourceView.Reset();
	depthStencilView.Reset();
	renderTargetView->Reset();
	swapchain.Reset();
	deviceContext.Reset();
	device.Reset();
}