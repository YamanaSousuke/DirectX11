#include "Transform.h"

using namespace DirectX;

// ç¿ïWÇÃê›íË
void Transform::SetPosition(const XMFLOAT3& position)
{
	this->position = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}

// ç¿ïWÇÃéÊìæ
XMFLOAT3 Transform::GetPosition() const
{
	return position;
}

DirectX::XMVECTOR Transform::GetPositionVector() const
{
	return XMLoadFloat3(&position);
}


