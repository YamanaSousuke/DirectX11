#pragma once

#include <d3d11.h>
#include <wrl/client.h>

// �T���v���[�A�u�����h�A���X�^���C�U�X�e�[�g������
class RenderState
{
public:
	// ������
	static bool InitAll(ID3D11Device* device);

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// ���`��Ԃ̃T���v�����O
	static ComPtr<ID3D11SamplerState> linerSamplerState;
};
