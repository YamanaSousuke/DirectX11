#include "Camera.h"

using namespace DirectX;

// �r���[�s��̐ݒ�
void Camera::SetViewMatrix(const XMVECTOR& position, const XMVECTOR& focus, const XMVECTOR& up)
{
	transform.SetPosition(position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	view = XMMatrixLookAtLH(position, focus, up);
}

// �r���[�s��̎擾
XMMATRIX Camera::GetViewMatrix() const
{
	return view;
}

// ������̐ݒ�
void Camera::SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ)
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
