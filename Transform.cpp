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

// 座標の取得
XMFLOAT3 Transform::GetPosition() const
{
	return position;
}

DirectX::XMVECTOR Transform::GetPositionVector() const
{
	return XMLoadFloat3(&position);
}


