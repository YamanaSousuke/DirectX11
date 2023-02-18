#pragma once

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Lightings.h"
#include "RenderState.h"

// �V�F�[�_�[�ƒ萔�o�b�t�@�[���Ǘ�����
class Effect
{
public:
	// ���\�[�X�̏�����
	bool InitAll(ID3D11Device* device);
	// �r���[�s��̐ݒ�
	void SetViewMatrix(const DirectX::XMMATRIX& view);
	// �v���W�F�N�V�����s��̐ݒ�
	void SetProjectionMatrix(const DirectX::XMMATRIX& projection);
	// �O��̃t���[������̌o�ߎ��Ԃ̐ݒ�
	void SetTime(float time);

	void SetRandom(int random);
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

	// �t�H�O�ɂ��Ă̐ݒ�
	void SetFogColor(const DirectX::XMFLOAT3& color);
	void SetFogState(bool enable);
	void SetFogStart(float start);
	void SetFogRange(float range);

	// ���ӃG�t�F�N�g�ɂ��Ă̐ݒ�
	void SetInitialVelocity(const DirectX::XMFLOAT4 initialVelocity);
	void SetIntencity(float intencity);

	// �f�t�H���g�̕`��
	void RenderDefault(ID3D11DeviceContext* immediateContext);
	// �@���̕`��
	void ShowNormaLine(ID3D11DeviceContext* immediateContext);
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �V�[�����
	struct SceneParameter {
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		float time;
		int randomTest;
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

	// �t�H�O
	struct FogParameter {
		DirectX::XMFLOAT3 fogColor;
		int fogEnable;
		float fogStart;
		float fogRange;
	};

	// ���ӃG�t�F�N�g
	struct CrushParamater {
		DirectX::XMFLOAT4 initialVelocity;
		float intencity;
	};

	// �萔�o�b�t�@�[�œ]������f�[�^
	enum class Data
	{
		Scene,
		Model,
		Light,
		Fog,
		Crush,
	};

	ConstantBufferObject<0, SceneParameter> sceneParameter = {};
	ConstantBufferObject<1, ModelParameter> modelParameter = {};
	ConstantBufferObject<2, LightParameter> lightParameter = {};
	ConstantBufferObject<3, FogParameter> fogParameter = {};
	ConstantBufferObject<4, CrushParamater> crushParameter = {};

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
