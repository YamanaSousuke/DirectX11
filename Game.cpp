#include <d3d11.h>
#include "Game.h"
#include <wrl/client.h>

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
	RECT rect = { 0, 0, 640, 480 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0, className, L"Title", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		OutputDebugStringA("�E�B���h�E�̍쐬�Ɏ��s\n");
		return false;
	}

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, SW_SHOWNORMAL);
	// �N���C�A���g�̈�̍X�V
	UpdateWindow(hWnd);

	return true;
}

// ���b�Z�[�W���[�v�̎��s
int Game::Run()
{
	// �E�B���h�E�̍쐬�Ɏ��s
	if (!InitWindow()) {
		return-1;
	}

	// �f�o�C�X
	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	// �f�o�C�X�R���e�L�X�g
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};

	HRESULT hr = S_OK;

	// �f�o�C�X�쐬���̃I�v�V�����t���O
	UINT crationFlag = 0;
#if defined(_DEBUG)
	crationFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// �쐬����@�\���x��
	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// �f�o�C�X�̍쐬
	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, crationFlag, featureLevels, _countof(featureLevels),
		D3D11_SDK_VERSION, &device, &featureLevel, &deviceContext);
	if (FAILED(hr))
	{
		OutputDebugStringA("�f�o�C�X�̍쐬�Ɏ��s\n");
		return -1;
	}

	// ���b�Z�[�W���[�v
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W���擾
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