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
	// ������
	bool Init();
	// �V�[���̍X�V
	void UpdateScene() override;
	// �V�[���̕`��
	void DrawScene() override;
private:
	// �J����
	Camera camera = {};
	// �G�t�F�N�g
	Effect effect = {};
	// FBX�t�@�C���̓ǂݍ���
	FbxMeshFile fbxMeshfile = {};
	//	�I�u�W�F�N�g
	GameObject house;

	// �t�H�O�ɂ��Ă̐ݒ�
	bool fogEnable = false;
	DirectX::XMFLOAT3 fogColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float fogStart = 0.0f;
	float fogEnd = 40.0f;

	// �^�C�}�[
	float time = 0.0f;
};