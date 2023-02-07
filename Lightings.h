#pragma once

#include <DirectXMath.h>

// ���C�g�S�ʂ̊Ǘ�

// �f�B���N�V�������C�g
struct DirectionalLight {
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 direction;
};

// �}�e���A��
struct Material {
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 specular;
	float smooth;
	float metallic;
};