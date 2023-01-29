#include "Transform.h"

using namespace DirectX;

// ���W�̐ݒ�
void Transform::SetPosition(const XMFLOAT3& position)
{
	this->position = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}

// ��]�̐ݒ�
void Transform::SetRotation(const DirectX::XMFLOAT3 eulerAngles)
{
	auto quaternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&eulerAngles));
	return XMStoreFloat3(&rotation, quaternion);
}

// ���W�̎擾
XMFLOAT3 Transform::GetPosition() const
{
	return position;
}

XMVECTOR Transform::GetPositionVector() const
{
	return XMLoadFloat3(&position);
}

// ���[���h�ϊ��s��̎擾
XMMATRIX Transform::GetWorldMatrix() const
{
	auto scaleVecrtor = XMLoadFloat3(&scale);
	auto rotationVector = XMLoadFloat3(&rotation);
	auto positionVector = XMLoadFloat3(&position);
	return XMMatrixScaling(scale.x, scale.y, scale.z) 
		* XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
		* XMMatrixTranslation(position.x, position.y, position.z);
}


