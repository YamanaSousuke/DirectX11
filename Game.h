#pragma once

#include <Windows.h>

// �A�v���P�[�V�����S�̂�\��
class Game {
	// �E�B���h�E�̃^�C�g��
	LPCWSTR title = L"Title";
	// �E�B���h�E�̕�
	int width = 640;
	// �E�B���h�E�̍���
	int height = 480;
	// �E�B���h�E�n���h��
	HWND hWnd = NULL;

public:
	// �A�v���P�[�V�����̏�����
	void Initialize(LPCWSTR title, int width, int height);
	// ���b�Z�[�W���[�v�̎��s
	int Run();

private:
	// �E�B���h�E�̍쐬
	bool InitWindow();
};