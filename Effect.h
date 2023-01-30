#pragma once

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Lightings.h"

// �萔�o�b�t�@�[����ăG�t�F�N�g���s��
class Effect
{
public:
	// ���\�[�X�̏�����
	bool InitAll(ID3D11Device* device);

	// �r���[�s��̐ݒ�
	void SetViewMatrix(const DirectX::XMVECTOR& eye, const DirectX::XMVECTOR& focus, const DirectX::XMVECTOR& up);
	// �v���W�F�N�V�����s��̐ݒ�
	void SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ);
	// ���[���h�s��̐ݒ�
	void SetWorldMatrix(const DirectX::XMMATRIX& matrix);
	// �}�e���A���̐ݒ�
	void SetMaterial(const Material& material);
	// �f�B���N�V���i�����C�g�̐ݒ�
	void SetDirectionalLight(UINT index, const DirectionalLight& directionalLight);
	// ���_�̐ݒ�
	void SetEyePosition(const DirectX::XMFLOAT3& position);
	// �萔�o�b�t�@�[�ƃe�N�X�`�����̓K��
	void Apply(ID3D11DeviceContext* immediateContext);

	// �f�t�H���g�̕`��
	void RenderDefault(ID3D11DeviceContext* immediateContext);
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �V�[�����
	struct SceneParameter {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	// ���f�����
	struct ModelParameter {
		DirectX::XMFLOAT4X4 world;
		Material material = {};
	};

	// ���C�g���
	struct LightParameter {
		DirectionalLight directionalLight[4] = {};
		DirectX::XMFLOAT3 eyePosition;
	};

	enum class Data
	{
		Scene,
		Model,
		Light,
	};

	ConstantBufferObject<0, SceneParameter> sceneParameter = {};
	ConstantBufferObject<1, ModelParameter> modelParameter = {};
	ConstantBufferObject<2, LightParameter> lightParameter = {};

	// �萔�o�b�t�@�[���܂Ƃ߂ĊǗ�����
	std::vector<ConstantBufferBase*> constantBuffers = { nullptr };

	// ���_�V�F�[�_�[
	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	// �W�I���g���V�F�[�_�[
	ComPtr<ID3D11GeometryShader> geometryShader = nullptr;
	// �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	// �C���v�b�g���C�A�E�g
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
};
