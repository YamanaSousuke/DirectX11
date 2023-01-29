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
void Transform::SetRotation(const DirectX::XMFLOAT3 eulerAngles)
{
	auto quaternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&eulerAngles));
	return XMStoreFloat3(&rotation, quaternion);
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
	return XMMatrixScaling(scale.x, scale.y, scale.z) 
		* XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
		* XMMatrixTranslation(position.x, position.y, position.z);
}


