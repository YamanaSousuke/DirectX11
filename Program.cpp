#include "Game.h"

// �G���g���[�|�C���g
// int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
// {
// 	Game game;
// 	// ������
// 	game.Initialize(L"DirectX11", 928, 522);
// 	// ���b�Z�[�W���[�v�̎��s
// 	return game.Run();
// }

int main()
{
	Game game;
	// ������
	game.Initialize(L"DirectX11", 928, 522);
	// ���b�Z�[�W���[�v�̎��s
	return game.Run();
}