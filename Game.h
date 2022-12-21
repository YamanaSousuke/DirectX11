#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

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
	// ��ʂ��N���A����Ƃ��Ɏg�p����J���[
	const FLOAT clearColor[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
	// �r���[�|�[�g
	D3D11_VIEWPORT viewports[1] = {};


	// �E�B���h�E�̍쐬
	bool InitWindow();
	// �O���t�B�b�N�f�o�C�X�̍쐬
	bool InitGraphicsDevice();
	// ���\�[�X�̉��
	void Release();
};

// ���_�V�F�[�_�[
class VertexShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	static VertexShader* Create(ID3D11Device* device);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11VertexShader* GetNativePointer();
	// ���\�[�X�̉��
	void Release();
};

// �W�I���g���V�F�[�_�[
class GeometryShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometryShader = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	static GeometryShader* Create(ID3D11Device* device);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11GeometryShader* GetNativePointer();
	// ���\�[�X�̉��
	void Release();
};

// �s�N�Z���V�F�[�_�[
class PixelShader {
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader = nullptr;

public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	static PixelShader* Create(ID3D11Device* device);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11PixelShader* GetNativePointer();
	// ���\�[�X�̉��
	void Release();
};

// ���_�o�b�t�@�[
class VertexBuffer
{
	//  ���\�[�X
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	static VertexBuffer* Create(ID3D11Device* device, UINT byteWidth);
	// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
	void SetData(void* data);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11Buffer* GetNativePointer();
	// ���\�[�X�̉��
	void Release();
};

// �C���f�b�N�X�o�b�t�@�[
class IndexBuffer
{
	//  ���\�[�X
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	static IndexBuffer* Create(ID3D11Device* device, UINT indexCount);
	// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
	void SetData(UINT16* data);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11Buffer* GetNativePointer();
	// ���\�[�X�̉��
	void Release();
};

// �萔�o�b�t�@�[
class ConstantBuffer
{
	//  ���\�[�X
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
public:
	// ���̃N���X�̐V�����C���X�^���X�̍쐬
	static ConstantBuffer* Create(ID3D11Device* device, UINT byteWidth);
	// �o�b�t�@�[�Ƀf�[�^��ݒ肷��
	void SetData(void* data);
	// �l�C�e�B�u�|�C���^�[�̎擾
	ID3D11Buffer* GetNativePointer();
	// ���\�[�X�̉��
	void Release();
};