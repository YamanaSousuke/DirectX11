#include "Game.h"

namespace {
	// 位置情報のみ
	const D3D11_INPUT_ELEMENT_DESC VertexPosition_InputElementDescs[1] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// 位置情報と法線情報
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormal_InputElementDescs[2] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,							   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// 位置情報と法線情報とテクスチャ座標
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTexture_InputElementDescs[3] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0,						   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
}

// インプットレイアウトの配列の取得
const D3D11_INPUT_ELEMENT_DESC* VertexPosition::GetInputElementDescs()
{
	return VertexPosition_InputElementDescs;
}

// インプットレイアウトの配列の要素数の取得
UINT VertexPosition::GetInputElementDescsLength()
{
	return _countof(VertexPosition_InputElementDescs);
}

// インプットレイアウトの配列の取得
const D3D11_INPUT_ELEMENT_DESC* VertexPositionNormal::GetInputElementDescs()
{
	return VertexPositionNormal_InputElementDescs;
}

// インプットレイアウトの配列の要素数の取得
UINT VertexPositionNormal::GetInputElementDescsLength()
{
	return _countof(VertexPositionNormal_InputElementDescs);
}

// インプットレイアウトの配列の取得
const D3D11_INPUT_ELEMENT_DESC* VertexPositionNormalTexture::GetInputElementDescs()
{
	return VertexPositionNormalTexture_InputElementDescs;
}

// インプットレイアウトの配列の要素数の取得
UINT VertexPositionNormalTexture::GetInputElementDescsLength()
{
	return _countof(VertexPositionNormalTexture_InputElementDescs);
}
