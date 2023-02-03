#pragma once

#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>

// ���_�\������уC���v�b�g���C�A�E�g

// ���_���̂�
struct VertexPosition {
	DirectX::XMFLOAT3 position;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[1];
};

// ���_�A�J���[
struct VertexPositionColor {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

// ���_�A�@���A�J���[
struct VertexPositionNormalColor {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};

// ���_�A�e�N�X�`���[
struct VertexPositionTexture {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texCoord;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

// ���_�A�@���A�e�N�X�`���[
struct VertexPositionNormalTexture {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texCoord;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};
