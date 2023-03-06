#include "Game.h"

// エントリーポイント
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
	// 16 : 9の比率で初期化
	Game game(L"DirectX11", 1200, 675);
	if (!game.Init()) {
		return 0;
	}
	
	// メッセージループの実行
	return game.Run();
}