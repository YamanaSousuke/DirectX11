#pragma once

#include <DirectXMath.h>

// トランスフォーム
class Transform {
public:
	// 座標の設定
	void SetPosition(const DirectX::XMFLOAT3& position);
	void SetPosition(float x, float y, float z);

	// 回転の設定
	void SetRotation(const DirectX::XMFLOAT3& eulerAngles);
	void SetRotation(float x, float y, float z);

	// 拡大縮小の設定
	void SetScale(const DirectX::XMFLOAT3& scale);
	void SetScale(float x, float y, float z);

	// 座標の取得
	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMVECTOR GetPositionVector() const;

	// 回転の取得
	DirectX::XMFLOAT3 GetRotation() const;

	// スケールの取得
	DirectX::XMFLOAT3 GetScale() const;

	// ワールド変換行列の取得
	DirectX::XMMATRIX GetWorldMatrix() const;
	// ワールド変換逆行列の取得
	DirectX::XMMATRIX GetWorldInverseMatrix() const;
private:
	// 座標
	DirectX::XMFLOAT3 position = {};
	// 回転
	DirectX::XMFLOAT3 rotation = {};
	// 拡大縮小
	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
};
