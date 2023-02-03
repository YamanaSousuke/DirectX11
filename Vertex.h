#pragma once

#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>

// 頂点構造およびインプットレイアウト

// 頂点情報のみ
struct VertexPosition {
	DirectX::XMFLOAT3 position;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[1];
};

// 頂点、カラー
struct VertexPositionColor {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

// 頂点、法線、カラー
struct VertexPositionNormalColor {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};

// 頂点、テクスチャー
struct VertexPositionTexture {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texCoord;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

// 頂点、法線、テクスチャー
struct VertexPositionNormalTexture {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texCoord;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};
