#pragma once

#include "D3DApp.h"
#include "Camera.h"
#include "GameObject.h"

// �A�v���P�[�V�����S�̂�\��
class Game : public D3DApp
{
public:
	// �R���X�g���N�^
	Game(LPCWSTR title, UINT width, UINT height);
	// ���b�Z�[�W���[�v�̎��s
	int Run();

private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �E�B���h�E�̃^�C�g��
	LPCWSTR title = L"Title";
	// �E�B���h�E�̕�
	int width = 640;
	// �E�B���h�E�̍���
	int height = 480;

	// �J����
	Camera camera = {};
	// �G�t�F�N�g
	Effect effect = {};
	// FBX
	FbxMeshFile fbxMeshfile = {};

	// �t�H�O�ɂ��Ă̐ݒ�
	bool fogEnable = false;
	DirectX::XMFLOAT3 fogColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float fogStart = 0.0f;
	float fogEnd = 40.0f;
	// GUI�̏�����
	bool InitGUI();
};