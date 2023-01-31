#pragma once

#include<DirectXMath.h>
#include "Transform.h"

// カメラ
class Camera
{
public:
	// ビュー行列の設定
	void SetViewMatrix(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);
	// ビュー行列の取得
	DirectX::XMMATRIX GetViewMatrix() const;

	// トランスフォームの取得
	Transform& GetTransform();
private:
	// トランスフォーム
	Transform transform = {};
};