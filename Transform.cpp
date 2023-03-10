#include "Transform.h"
#include <math.h>

using namespace DirectX;

// コンストラクター
Transform::Transform(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& scale)
	: position(position), rotation(rotation), scale(scale)
{

}

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
void Transform::SetScale(const XMFLOAT3& scale)
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

// 回転の取得
XMFLOAT3 Transform::GetRotation() const
{
	return rotation;
}

// 度で回転の取得
XMFLOAT3 Transform::GetRotationInDegree() const
{
	return XMFLOAT3(float(fmod(XMConvertToDegrees(rotation.x), 360.0f)), 
		float(fmod(XMConvertToDegrees(rotation.y), 360.0f)), 
		float(fmod(XMConvertToDegrees(rotation.z), 360.0f)));
}

// スケールの取得
XMFLOAT3 Transform::GetScale() const
{
	return scale;
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


