#include "Camera.h"

using namespace DirectX;

// �r���[�s��̐ݒ�
void Camera::SetViewMatrix(const XMFLOAT3& position, const XMFLOAT3& focus, const XMFLOAT3& up)
{
	transform.SetPosition(position);
	// XMMatrixLookAtLH(eye, focus, up);
}

// �r���[�s��̎擾
XMMATRIX Camera::GetViewMatrix() const
{
	return transform.GetWorldInverseMatrix();
}


// �g�����X�t�H�[���̎擾
Transform& Camera::GetTransform()
{
	return transform;
}
