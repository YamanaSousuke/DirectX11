#include "Game.h"

// �G���g���[�|�C���g
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
	// 16 : 9�̔䗦�ŏ�����
	Game game(L"DirectX11", 1200, 675);
	if (!game.Init()) {
		return 0;
	}
	
	// ���b�Z�[�W���[�v�̎��s
	return game.Run();
}