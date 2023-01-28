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

// ���W�̎擾
XMFLOAT3 Transform::GetPosition() const
{
	return position;
}

DirectX::XMVECTOR Transform::GetPositionVector() const
{
	return XMLoadFloat3(&position);
}


