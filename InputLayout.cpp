#include "Game.h"

// ���̃N���X�̐V�����C���X�^���X�̍쐬
InputLayout* InputLayout::Create(ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC* descs, UINT numElements,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// ���̃N���X�̃������[���m��
	auto result = new InputLayout();
	if (result == nullptr) {
		return nullptr;
	}

	// �C���v�b�g���C�A�E�g�̍쐬
	auto hr = device->CreateInputLayout(descs, numElements, shaderBytecode, bytecodeLength, &result->inputLayout);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// �l�C�e�B�u�|�C���^�[�̎擾
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return inputLayout.Get();
}

// ���\�[�X�̉��
void InputLayout::Release()
{
	delete this;
}
