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
	// ������
	Game game(L"DirectX11", static_cast<UINT>(1024 * 1.05f), static_cast<UINT>(576 * 1.2f));
	if (!game.Init()) {
		return 0;
	}

	// ���b�Z�[�W���[�v�̎��s
	return game.Run();
}