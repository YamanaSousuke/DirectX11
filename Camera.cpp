#include "Camera.h"

using namespace DirectX;

// ビュー行列の設定
void Camera::SetViewMatrix(const XMFLOAT3& position, const XMFLOAT3& focus, const XMFLOAT3& up)
{
	transform.SetPosition(position);
	// XMMatrixLookAtLH(eye, focus, up);
}

// ビュー行列の取得
XMMATRIX Camera::GetViewMatrix() const
{
	return transform.GetWorldInverseMatrix();
}

// 視錐台の設定
void Camera::SetFrustum(float fov, float aspect, float nearZ, float farZ)
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
