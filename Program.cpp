#include "Game.h"

// エントリーポイント
// int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
// {
// 	Game game;
// 	// 初期化
// 	game.Initialize(L"DirectX11", 928, 522);
// 	// メッセージループの実行
// 	return game.Run();
// }

int main()
{
	Game game;
	// 初期化
	game.Initialize(L"DirectX11", 928, 522);
	// メッセージループの実行
	return game.Run();
}