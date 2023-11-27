#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

#include <windows.h>
#include <tchar.h>

#include "game.h"
#include "tools.h"

using namespace std;

Game game;
Tools tools;
bool gamestart = false;


LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch (iMsg) {
	case WM_CREATE:
		game.initGame(hwnd);
		break;

	case WM_KEYDOWN:
		
		if (wParam == VK_RETURN) {
			game.startGame();
		}
		
		if (wParam == VK_SPACE) {
			gamestart = true;
			SetTimer(hwnd, 1, 100, NULL);
		}
		
		game.setDirect(wParam);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (gamestart) {
			if (game.moveSnake(hdc)) KillTimer(hwnd, 1);
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		game.endGame();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdLine, _In_ int nCmdShow) {

	HWND     hwnd;
	MSG		 msg;
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = _T("Window Class Name");

	RegisterClass(&WndClass);

	hwnd = CreateWindow(
		_T("Window Class Name"),
		_T("Snake Game"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1200,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
