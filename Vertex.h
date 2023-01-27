#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// ���_�\������уC���v�b�g���C�A�E�g

// ���_���̂�
struct VertexPosition {
	DirectX::XMFLOAT3 position;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[1];
};

// ���_�A�@���A�e�N�X�`���[
struct VertexPositionNormalTexture {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texCoord;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};
