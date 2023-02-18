#pragma once

#include<DirectXMath.h>
#include "Transform.h"

// カメラ
class Camera
{
public:
	// ビュー行列の設定
	void SetViewMatrix(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& focus, const DirectX::XMVECTOR& up);
	// ビュー行列の取得
	DirectX::XMMATRIX GetViewMatrix() const;
	// プロジェクション行列の設定
	void SetProjectionMatrix(float fov, float aspect, float nearZ, float farZ);
	// プロジェクション行列の取得
	DirectX::XMMATRIX GetProjectionMatrix() const;

	// トランスフォームの取得
	Transform& GetTransform();
private:
	// トランスフォーム
	Transform transform = {};
	// 
	DirectX::XMMATRIX view;

	// 視野
	float fov = 0.0f;
	// アスペクト比
	float aspect = 0.0f;
	// ニアクリッピング
	float nearZ = 0.0f;
	// ファークリッピング
	float farZ = 0.0f;
};