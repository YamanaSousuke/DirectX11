#pragma once

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Lightings.h"

// �|���S���ɑ΂��ăG�t�F�N�g������
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

	// �萔�o�b�t�@�[�ƃe�N�X�`�����̓K��
	void Apply(ID3D11DeviceContext* immediateContext);


private:
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

	enum class Data
	{
		Scene,
		Model,
	};

	ConstantBufferObject<0, SceneParameter> sceneParameter = {};
	ConstantBufferObject<1, ModelParameter> modelParameter = {};

	// �萔�o�b�t�@�[���܂Ƃ߂ĊǗ�����
	std::vector<ConstantBufferBase*> constantBuffers = { nullptr };
};
