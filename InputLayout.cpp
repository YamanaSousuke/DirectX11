#include "Game.h"

// このクラスの新しいインスタンスの作成
InputLayout::InputLayout(ID3D11Device* device, const D3D11_INPUT_ELEMENT_DESC* descs, UINT numElements,
	const void* shaderBytecode, SIZE_T bytecodeLength)
{
	const auto hr = device->CreateInputLayout(descs, numElements, shaderBytecode, bytecodeLength, &inputLayout);
	if (FAILED(hr)) {
		OutputDebugString(L"インプットレイアウトの作成に失敗\n");
	}
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
