#include "Transform.h"

using namespace DirectX;

// 座標の設定
void Transform::SetPosition(const XMFLOAT3& position)
{
	this->position = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}

// 回転の設定
void Transform::SetRotation(const XMFLOAT3& eulerAngles)
{
	rotation = eulerAngles;
}

void Transform::SetRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
}

// 拡大縮小の設定
void Transform::SetScale(const DirectX::XMFLOAT3& scale)
{
	this->scale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
}

// 座標の取得
XMFLOAT3 Transform::GetPosition() const
{
	return position;
}

XMVECTOR Transform::GetPositionVector() const
{
	return XMLoadFloat3(&position);
}

// ワールド変換行列の取得
XMMATRIX Transform::GetWorldMatrix() const
{
	auto scaleVecrtor = XMLoadFloat3(&scale);
	auto rotationVector = XMLoadFloat3(&rotation);
	auto positionVector = XMLoadFloat3(&position);
	return XMMatrixScalingFromVector(scaleVecrtor)
		* XMMatrixRotationRollPitchYawFromVector(rotationVector)
		* XMMatrixTranslationFromVector(positionVector);
}

// ワールド変換逆行列の取得
XMMATRIX Transform::GetWorldInverseMatrix() const
{
	return XMMatrixInverse(nullptr, GetWorldMatrix());
}


