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

// ������̐ݒ�
void Camera::SetFrustum(float fov, float aspect, float nearZ, float farZ)
{
	this->fov = fov;
	this->aspect = aspect;
	this->nearZ = nearZ;
	this->farZ = farZ;
}

// �v���W�F�N�V�����s��̎擾
XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
}

// �g�����X�t�H�[���̎擾
Transform& Camera::GetTransform()
{
	return transform;
}
