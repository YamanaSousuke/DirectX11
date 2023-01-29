#pragma once

#include <DirectXMath.h>

// トランスフォーム
class Transform {
public:
	// 座標の設定
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetPosition(float x, float y, float z);

	// 回転の設定
	void SetRotation(const DirectX::XMFLOAT3 eulerAngles);

	// 座標の取得
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMVECTOR GetPositionVector() const;

	// ワールド変換行列の取得
	DirectX::XMMATRIX GetWorldMatrix() const;
private:
	// 座標
	DirectX::XMFLOAT3 position = {};
	// 回転
	DirectX::XMFLOAT3 rotation = {};
	// 拡大縮小
	DirectX::XMFLOAT3 scale = {};
};
