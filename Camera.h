#pragma once

#include<DirectXMath.h>
#include "Transform.h"

// �J����
class Camera
{
public:
	// �r���[�s��̐ݒ�
	void SetViewMatrix(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);
	// �r���[�s��̎擾
	DirectX::XMMATRIX GetViewMatrix() const;

	// �g�����X�t�H�[���̎擾
	Transform& GetTransform();
private:
	// �g�����X�t�H�[��
	Transform transform = {};
};