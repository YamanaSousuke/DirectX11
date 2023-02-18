#include "Camera.h"

using namespace DirectX;

// ビュー行列の設定
void Camera::SetViewMatrix(const XMVECTOR& position, const XMVECTOR& focus, const XMVECTOR& up)
{
	transform.SetPosition(position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	view = XMMatrixLookAtLH(position, focus, up);
}

// ビュー行列の取得
XMMATRIX Camera::GetViewMatrix() const
{
	return view;
}

// 視錐台の設定
void Camera::SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ)
{
	this->fov = fov;
	this->aspect = aspect;
	this->nearZ = nearZ;
	this->farZ = farZ;
}

// プロジェクション行列の取得
XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
}

// トランスフォームの取得
Transform& Camera::GetTransform()
{
	return transform;
}
