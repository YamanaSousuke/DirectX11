#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "FBXMeshFile.h"
#include "GameObject.h"

#include "Lightings.h"
#include "Effect.h"

// �A�v���P�[�V�����S�̂�\��
class Game
{
public:
	// �A�v���P�[�V�����̏�����
	void Initialize(LPCWSTR title, int width, int height);
	// ���b�Z�[�W���[�v�̎��s
	int Run();

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �E�B���h�E�̃^�C�g��
	LPCWSTR title = L"Title";
	// �E�B���h�E�̕�
	int width = 640;
	// �E�B���h�E�̍���
	int height = 480;
	// �E�B���h�E�n���h��
	HWND hWnd = NULL;
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

	// �G�t�F�N�g
	Effect effect = {};
	// FBX
	FbxMeshFile fbxMeshfile = {};
	// ��
	GameObject house;


	// �t�H�O�ɂ��Ă̐ݒ�
	bool fogEnable = false;
	DirectX::XMFLOAT3 fogColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float fogStart = 0.0f;
	float fogEnd = 0.0f;

	// �E�B���h�E�̍쐬
	bool InitWindow();
	// �O���t�B�b�N�f�o�C�X�̍쐬
	bool InitGraphicsDevice();
	// GUI�̏�����
	bool InitGUI();
};