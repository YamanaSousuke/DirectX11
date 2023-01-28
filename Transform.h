#pragma once

#include <DirectXMath.h>

// トランスフォーム
class Transform {
public:
	// 座標の設定
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetPosition(float x, float y, float z);

	// 座標の取得
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMVECTOR GetPositionVector() const;
private:
	DirectX::XMFLOAT3 position;
};
