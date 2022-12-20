#include <Windows.h>

// エントリーポイント
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	// ウィンドウハンドル
	HWND hWnd = NULL;

	// ウィンドウクラスを登録する
	const wchar_t className[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = DefWindowProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = className;
	if (!RegisterClassEx(&wndClass))
	{
		OutputDebugStringA("ウィンドウクラスの登録に失敗\n");
		return -1;
	}

	// クライアント領域の指定
	RECT rect = { 0, 0, 640, 480 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0, L"Static", L"Title", 
		CW_USEDEFAULT, CW_USEDEFAULT, WS_OVERLAPPEDWINDOW, 
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		OutputDebugStringA("ウィンドウの作成に失敗\n");
		return -1;
	}

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	// クライアント領域の更新
	UpdateWindow(hWnd);
	return 0;
}