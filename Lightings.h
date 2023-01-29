#pragma once

#include <DirectXMath.h>

// ライト全般の管理

// ディレクションライト
struct DirectionalLight {
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 direction;
};

// マテリアル
struct Material {
	DirectX::XMFLOAT4 diffuse;
	float smooth;
	float metallic;
};