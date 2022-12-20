#include <Windows.h>


// ��M�������b�Z�[�W�ɉ���������
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		// �I������
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// �G���g���[�|�C���g
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	// �E�B���h�E�n���h��
	HWND hWnd = NULL;

	// �E�B���h�E�N���X��o�^����
	const wchar_t className[] = L"GameWindow";
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = WindowProc;
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = className;
	if (!RegisterClassEx(&wndClass))
	{
		OutputDebugStringA("�E�B���h�E�N���X�̓o�^�Ɏ��s\n");
		return -1;
	}

	// �N���C�A���g�̈�̎w��
	RECT rect = { 0, 0, 640, 480 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindowEx(0, className, L"Title", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 
		(rect.right - rect.left), (rect.bottom - rect.top),
		NULL, NULL, hInstance, NULL);
	if (hWnd == NULL)
	{
		OutputDebugStringA("�E�B���h�E�̍쐬�Ɏ��s\n");
		return -1;
	}

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	// �N���C�A���g�̈�̍X�V
	UpdateWindow(hWnd);

	// ���b�Z�[�W���[�v
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W���擾
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}