#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

class D3DApp
{
public:
	// �R���X�g���N�^
	D3DApp(LPCWSTR title, UINT width, UINT height) : title(title), width(width), height(height) { }
	// �f�X�g���N�^
	~D3DApp();
	// �S�̂̏�����
	virtual bool Init();
	// ��M�������b�Z�[�W�ɉ���������
	// LRESULT WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	// ���b�Z�[�W���[�v�̎��s
	int Run();
	// �V�[���̍X�V
	virtual void UpdateScene() = 0;
	// �V�[���̕`��
	virtual void DrawScene() = 0;
	// �E�B���h�E�̃T�C�Y���ύX���ꂽ�Ƃ��ɌĂяo�����
	virtual void OnResize();
	// �A�X�y�N�g��̎擾
	float AspectRatio() const;
protected:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �f�o�C�X
	ComPtr<ID3D11Device> device = nullptr;
	// �f�o�C�X�R���e�L�X�g
	ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// �X���b�v�`�F�[��
	ComPtr<IDXGISwapChain> swapchain = nullptr;
	// �����_�[�^�[�Q�b�g
	ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
	// �[�x�X�e���V��
	ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X�r���[
	ComPtr<ID3D11ShaderResourceView> depthStencilResourceView = nullptr;
	// �r���[�|�[�g
	D3D11_VIEWPORT viewports[1] = {};
	// �E�B���h�E�n���h��
	HWND hWnd = NULL;
	// �w�i�F
	const float backgroundColor[4] = { 0.0f, 0.0f, 0.9f, 1.0f };

	// �E�B���h�E�̏�����
	bool InitMainWindow();
	// �O���t�B�b�N�f�o�C�X�̏�����
	bool InitGraphicsDevice();
	// GUI�̏�����
	bool InitGUI();
private:
	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};
	// �[�x�X�e���V���̃t�H�[�}�b�g
	const DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

	// �E�B���h�E�^�C�g��
	LPCWSTR title = {};
	// ����
	UINT width = 0;
	// �c��
	UINT height = 0;
};
