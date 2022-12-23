#include "Game.h"

namespace {
	// �ʒu���̂�
	const D3D11_INPUT_ELEMENT_DESC VertexPosition_InputElementDescs[1] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �ʒu���Ɩ@�����
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormal_InputElementDescs[2] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,							   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// �ʒu���Ɩ@�����ƃe�N�X�`�����W
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTexture_InputElementDescs[3] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0,						   D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
}

// �C���v�b�g���C�A�E�g�̔z��̎擾
const D3D11_INPUT_ELEMENT_DESC* VertexPosition::GetInputElementDescs()
{
	return VertexPosition_InputElementDescs;
}

// �C���v�b�g���C�A�E�g�̔z��̗v�f���̎擾
UINT VertexPosition::GetInputElementDescsLength()
{
	return _countof(VertexPosition_InputElementDescs);
}

// �C���v�b�g���C�A�E�g�̔z��̎擾
const D3D11_INPUT_ELEMENT_DESC* VertexPositionNormal::GetInputElementDescs()
{
	return VertexPositionNormal_InputElementDescs;
}

// �C���v�b�g���C�A�E�g�̔z��̗v�f���̎擾
UINT VertexPositionNormal::GetInputElementDescsLength()
{
	return _countof(VertexPositionNormal_InputElementDescs);
}

// �C���v�b�g���C�A�E�g�̔z��̎擾
const D3D11_INPUT_ELEMENT_DESC* VertexPositionNormalTexture::GetInputElementDescs()
{
	return VertexPositionNormalTexture_InputElementDescs;
}

// �C���v�b�g���C�A�E�g�̔z��̗v�f���̎擾
UINT VertexPositionNormalTexture::GetInputElementDescsLength()
{
	return _countof(VertexPositionNormalTexture_InputElementDescs);
}
