#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

class D3DApp
{
public:
	// �R���X�g���N�^
	D3DApp(LPCWSTR title, UINT width, UINT height)
		: title(title), width(width), height(height) { }
	// ������
	bool Init();
	// �E�B���h�E�̏�����
	bool InitMainWindow();
	// �O���t�B�b�N�f�o�C�X�̍쐬
	bool InitGraphicsDevice();
	// �E�B���h�E�n���h��
	HWND hWnd = NULL;

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �f�o�C�X
	ComPtr<ID3D11Device> device = nullptr;
	// �f�o�C�X�R���e�L�X�g
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};
	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain> swapchain = nullptr;
	// �����_�[�^�[�Q�b�g
	ComPtr<ID3D11RenderTargetView> renderTargetView[1];
	// �[�x�X�e���V��
	ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X�r���[
	ComPtr<ID3D11ShaderResourceView> depthStencilResourceView = nullptr;
	// �[�x�X�e���V���̃t�H�[�}�b�g
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	// �r���[�|�[�g
	D3D11_VIEWPORT viewports[1] = {};
private:
	// �E�B���h�E�n���h��
	HINSTANCE hInstance = {};
	
	// �E�B���h�E�^�C�g��
	LPCWSTR title = {};
	// ����
	UINT width = 0;
	// �c��
	UINT height = 0;
};
