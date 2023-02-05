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
	// �E�B���h�E�̃^�C�g��
	LPCWSTR title = L"Title";
	// �E�B���h�E�̕�
	int width = 640;
	// �E�B���h�E�̍���
	int height = 480;
	// �E�B���h�E�n���h��
	HWND hWnd = NULL;
	// �f�o�C�X
	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	// �f�o�C�X�R���e�L�X�g
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	// �@�\���x��
	D3D_FEATURE_LEVEL featureLevel = {};
	// �X���b�v�`�F�[��
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain = nullptr;
	// �����_�[�^�[�Q�b�g
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView[1];
	// �[�x�X�e���V��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
	// �[�x�X�e���V�����V�F�[�_�[�ŗ��p���邽�߂̃��\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depthStencilResourceView = nullptr;
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

// 2D�e�N�X�`���[
class Texture2D
{
	// ���\�[�X
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
	// �T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState = nullptr;
	// �V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	Texture2D(ID3D11Device* device, UINT width, UINT height,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, bool mipChain = true);
	// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
	void SetData(const void* data);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11Texture2D* GetNativePointer();
	ID3D11SamplerState* GetSapmlerState();
	ID3D11ShaderResourceView* GetShaderResourceView();
	// ���\�[�X�̉��
	void Release();
};

// ���X�^���C�U�X�e�[�g
class RasterizerState
{
	// ���\�[�X
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> state = nullptr;
public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	static RasterizerState* Create(ID3D11Device* device);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11RasterizerState* GetNativePointer();
	// ���\�[�X�̉��
	void Release();
};

// �u�����h�X�e�[�g
class BlendState
{
	// ���\�[�X
	Microsoft::WRL::ComPtr<ID3D11BlendState> state = nullptr;
public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	static BlendState* Create(ID3D11Device* device);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11BlendState* GetNativePointer();
	// BlendFactor�̎擾
	const FLOAT* GetBlendFactor();
	// ���\�[�X�̉��
	void Release();
};