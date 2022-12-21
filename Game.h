#pragma once

#include <Windows.h>

// アプリケーション全体を表す
class Game {
	// ウィンドウのタイトル
	LPCWSTR title = L"Title";
	// ウィンドウの幅
	int width = 640;
	// ウィンドウの高さ
	int height = 480;
	// ウィンドウハンドル
	HWND hWnd = NULL;

public:
	// アプリケーションの初期化
	void Initialize(LPCWSTR title, int width, int height);
	// メッセージループの実行
	int Run();

private:
	// ウィンドウの作成
	bool InitWindow();
};