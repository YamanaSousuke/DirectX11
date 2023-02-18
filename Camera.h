#pragma once

#include<DirectXMath.h>
#include "Transform.h"

// �J����
class Camera
{
public:
	// �r���[�s��̐ݒ�
	void SetViewMatrix(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& focus, const DirectX::XMVECTOR& up);
	// �r���[�s��̎擾
	DirectX::XMMATRIX GetViewMatrix() const;
	// �v���W�F�N�V�����s��̐ݒ�
	void SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ);
	// �v���W�F�N�V�����s��̎擾
	DirectX::XMMATRIX GetProjectionMatrix() const;

	// �g�����X�t�H�[���̎擾
	Transform& GetTransform();
private:
	// �g�����X�t�H�[��
	Transform transform = {};
	// 
	DirectX::XMMATRIX view;

	// ����
	float fov = 0.0f;
	// �A�X�y�N�g��
	float aspect = 0.0f;
	// �j�A�N���b�s���O
	float nearZ = 0.0f;
	// �t�@�[�N���b�s���O
	float farZ = 0.0f;
};