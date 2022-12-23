#include "Game.h"

// このクラスの新しいインスタンスの作成
InputLayout* InputLayout::Create(ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC* descs, UINT numElements,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	// このクラスのメモリーを確保
	auto result = new InputLayout();
	if (result == nullptr) {
		return nullptr;
	}

	// インプットレイアウトの作成
	auto hr = device->CreateInputLayout(descs, numElements, shaderBytecode, bytecodeLength, &result->inputLayout);
	if (FAILED(hr)) {
		return nullptr;
	}

	return result;
}

// ネイティブポインターの取得
ID3D11InputLayout* InputLayout::GetNativePointer()
{
	return inputLayout.Get();
}

// リソースの解放
void InputLayout::Release()
{
	delete this;
}
