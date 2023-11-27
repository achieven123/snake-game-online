#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

#include <windows.h>
#include <tchar.h>
#include "game.h"
#include "tools.h"

#define ID_SOLO_PLAYER 100
#define ID_MULTI_PLAYER 101
#define ID_SERVER_BACK_BUTTON 200
#define ID_SERVER_NEXT_BUTTON 201
#define ID_CREATE_ROOM 300
#define ID_JOIN_ROOM 301
#define ID_CODE_BACK_BUTTON 400
#define ID_CODE_NEXT_BUTTON 401
#define ID_GAME_START 500

using namespace std;

Game game;
Tools tools;
bool multi = false;
bool gameStart = false;

RECT rcClient;
HFONT hGameTitleFont, hRoomCommentFont;
int width, height;

HINSTANCE hInst;
HWND hwndMainMenu, hwndGame;
HWND hGameTitle, hSoloPlayerButton, hMultiPlayerButton;
HWND hIPAddressStatic, hPortStatic, hIPAddress, hPort, hServerBackButton, hServerNextButton;
HWND hRoomTypeStatoc, hCreateRoomButton, hJoinRoomButton;
HWND hRoomCodeStatic, hCodeStatic, hCode, hCodeBackButton, hCodeNextButton;
HWND hGameStartStatic, hStartRoomCodeStatic, hGameStartButton;

LPCTSTR MainMenuClass = TEXT("Main Menu Class");
LPCTSTR GameClass = TEXT("Server Info Class");
LPCTSTR JoinTypeClass = TEXT("Join Type Class");
LPCTSTR JoinRoomClass = TEXT("Join Room Class");
LPCTSTR SoloGameClass = TEXT("Solo Game Class");
LPCTSTR MultiGameClass = TEXT("Multi Game Class");

LRESULT CALLBACK WinProcMainMenu(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProcGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

void DrawSetting(HWND hwnd);
void DrawSelectMode(HWND hwnd);
void DrawServerInfo(HWND hwnd);
void DrawRoomType(HWND hwnd);
void DrawRoomCode(HWND hwnd);
void DrawGameStart(HWND hwnd);

void DeleteSelectMode();
void DeleteServerInfo();
void DeleteRoomType();
void DeleteRoomCode();
void DeleteGameStart();

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdLine, _In_ int nCmdShow) {
	MSG		 msg;
	WNDCLASS WndClass;

	hInst = hInstance;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;

	WndClass.lpszClassName = MainMenuClass;
	WndClass.lpfnWndProc = WinProcMainMenu;
	RegisterClass(&WndClass);

	WndClass.lpszClassName = GameClass;
	WndClass.lpfnWndProc = WinProcGame;
	RegisterClass(&WndClass);

	hwndMainMenu = CreateWindowEx(
		0,
		MainMenuClass,
		L"Snake Game",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		600, 400,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwndMainMenu, nCmdShow);

	hwndGame = CreateWindowEx(
		0,
		GameClass,
		L"Snake Game",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		955, 530,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwndGame, SW_HIDE);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WinProcMainMenu(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_CREATE:
		DrawSetting(hwnd);
		DrawSelectMode(hwnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_SOLO_PLAYER:
			multi = false;
			DeleteSelectMode();
			DrawGameStart(hwnd);
			break;

		case ID_MULTI_PLAYER:
			multi = true;
			DeleteSelectMode();
			DrawServerInfo(hwnd);
			break;

		case ID_SERVER_BACK_BUTTON:
			DeleteServerInfo();
			DrawSelectMode(hwnd);
			break;

		case ID_SERVER_NEXT_BUTTON:
			WCHAR _host[256];
			WCHAR _port[6];

			char host[256];
			char port[8];

			GetWindowText(hIPAddress, _host, 256);
			GetWindowText(hPort, _port, 6);

			WideCharToMultiByte(CP_UTF8, 0, _host, -1, host, 256, NULL, NULL);
			WideCharToMultiByte(CP_UTF8, 0, _port, -1, port, 8, NULL, NULL);
			
			bool connectSuccessful;
			connectSuccessful = game.initGame(hwnd, host, port);

			if (connectSuccessful) {
				DeleteServerInfo();
				DrawRoomType(hwnd);
			}

			break;

		case ID_CREATE_ROOM:
			game.createRoom();
			DeleteRoomType();
			DrawGameStart(hwnd);
			break;

		case ID_JOIN_ROOM:
			DeleteRoomType();
			DrawRoomCode(hwnd);
			break;

		case ID_CODE_BACK_BUTTON:
			DeleteRoomCode();
			DrawRoomType(hwnd);
			break;

		case ID_CODE_NEXT_BUTTON:
			WCHAR _code[7];

			char code[7];

			GetWindowText(hCode, _code, 256);

			WideCharToMultiByte(CP_UTF8, 0, _code, -1, code, 256, NULL, NULL);

			bool joinSuccessful;
			joinSuccessful = game.joinRoom(hwnd, code);

			if (joinSuccessful) {
				DeleteRoomCode();
				DrawGameStart(hwnd);
			}

			break;

		case ID_GAME_START:
			DeleteGameStart();
			ShowWindow(hwndMainMenu, SW_HIDE);
			ShowWindow(hwndGame, SW_SHOWNORMAL);

			break;
		}
		
		break;

	case WM_CTLCOLORSTATIC:
		HDC hdcStatic;
		hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);

	case WM_DESTROY:
		game.endGame();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK WinProcGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_CREATE:
		game.startGame(multi);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		if (gameStart) {
			game.moveSnake(hdc, multi);
			//if (game.moveSnake(hdc)) KillTimer(hwnd, 1);
		}

		TextOut(hdc, 520, 450, L"상대를 기다리는 중...", wcslen(L"상대를 기다리는 중..."));

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_SPACE) {
			SetTimer(hwnd, 1, 100, NULL);
		}

		game.setDirect(wParam);

		break;

	case WM_TIMER:
		InvalidateRgn(hwnd, NULL, TRUE);

		if (game.IsTwoPlayer()) {
			gameStart = true;
		}

		break;

	case WM_DESTROY:
		game.endGame();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void DrawSetting(HWND hwnd) {
	GetClientRect(hwnd, &rcClient);
	width = rcClient.right - rcClient.left;
	height = rcClient.bottom - rcClient.top;

	hGameTitleFont = CreateFont(
		40, 0, 0, 0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		NULL
	);

	hRoomCommentFont = CreateFont(
		30, 0, 0, 0,
		FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,
		VARIABLE_PITCH,
		NULL
	);
}

void DrawSelectMode(HWND hwnd) {
	hGameTitle = CreateWindow(
		L"STATIC",
		L"SNAKE GAME",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 300) / 2, (height - 60) / 2 - 20, 300, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hGameTitle, WM_SETFONT, (WPARAM)hGameTitleFont, TRUE);

	hSoloPlayerButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"Solo Player",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2, (height - 30) / 2 + 50, 100, 30,
		hwnd,
		(HMENU)ID_SOLO_PLAYER,
		hInst,
		NULL
	);

	hMultiPlayerButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"Multi Player",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2, (height - 30) / 2 + 100, 100, 30,
		hwnd,
		(HMENU)ID_MULTI_PLAYER,
		hInst,
		NULL
	);
}

void DrawServerInfo(HWND hwnd) {
	hGameTitle = CreateWindow(
		L"STATIC",
		L"SNAKE GAME",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 300) / 2, (height - 60) / 2 - 20, 300, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hGameTitle, WM_SETFONT, (WPARAM)hGameTitleFont, TRUE);

	hIPAddressStatic = CreateWindow(
		L"STATIC",
		L"IP 주소",
		WS_CHILD | WS_VISIBLE,
		(width - 50) / 2 - 100, (height - 60) / 2 + 50, 80, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	hPortStatic = CreateWindow(
		L"STATIC",
		L"Port 번호",
		WS_CHILD | WS_VISIBLE,
		(width - 50) / 2 - 100, (height - 60) / 2 + 100, 80, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	hIPAddress = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"113.198.230.13",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		(width - 150) / 2 + 50, (height - 25) / 2 + 25, 150, 25,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	hPort = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"8080",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		(width - 150) / 2 + 50, (height - 25) / 2 + 75, 150, 25,
		hwnd,
		NULL,
		hInst,
		NULL);

	hServerBackButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"< 뒤로",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 100, (height - 30) / 2 + 150, 100, 30,
		hwnd,
		(HMENU)ID_SERVER_BACK_BUTTON,
		hInst,
		NULL
	);

	hServerNextButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"다음 >",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 220, (height - 30) / 2 + 150, 100, 30,
		hwnd,
		(HMENU)ID_SERVER_NEXT_BUTTON,
		hInst,
		NULL
	);
}

void DrawRoomType(HWND hwnd) {
	hRoomTypeStatoc = CreateWindow(
		L"STATIC",
		L"멀티 참여 방법 선택",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 300) / 2, (height - 60) / 2 - 20, 300, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hRoomTypeStatoc, WM_SETFONT, (WPARAM)hRoomCommentFont, TRUE);

	hCreateRoomButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"Create Room",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 - 70, (height - 30) / 2 + 50, 100, 30,
		hwnd,
		(HMENU)ID_CREATE_ROOM,
		hInst,
		NULL
	);

	hJoinRoomButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"Join Room",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 70, (height - 30) / 2 + 50, 100, 30,
		hwnd,
		(HMENU)ID_JOIN_ROOM,
		hInst,
		NULL
	);
}

void DrawRoomCode(HWND hwnd) {
	hRoomCodeStatic = CreateWindow(
		L"STATIC",
		L"Room Code 입력",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 300) / 2, (height - 60) / 2 - 20, 300, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hRoomCodeStatic, WM_SETFONT, (WPARAM)hRoomCommentFont, TRUE);
	
	hCodeStatic = CreateWindow(
		L"STATIC",
		L"Room Code",
		WS_CHILD | WS_VISIBLE,
		(width - 50) / 2 - 100, (height - 60) / 2 + 50, 80, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	hCode = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		(width - 150) / 2 + 50, (height - 25) / 2 + 25, 150, 25,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	hCodeBackButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"< 뒤로",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 100, (height - 30) / 2 + 150, 100, 30,
		hwnd,
		(HMENU)ID_CODE_BACK_BUTTON,
		hInst,
		NULL
	);

	hCodeNextButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"다음 >",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 220, (height - 30) / 2 + 150, 100, 30,
		hwnd,
		(HMENU)ID_CODE_NEXT_BUTTON,
		hInst,
		NULL
	);
}

void DrawGameStart(HWND hwnd) {
	hGameStartStatic = CreateWindow(
		L"STATIC",
		L"게임을 시작하겠습니까?",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 350) / 2, (height - 60) / 2 - 20, 350, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hGameStartStatic, WM_SETFONT, (WPARAM)hRoomCommentFont, TRUE);
	
	const wchar_t* roomCode = tools.ConvertToWideString("Room Code: " + game.getSnakeCode());

	hStartRoomCodeStatic = CreateWindow(
		L"STATIC",
		roomCode,
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 300) / 2, (height - 60) / 2 + 25, 300, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	hGameStartButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"START",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2, (height - 30) / 2 + 50, 100, 30,
		hwnd,
		(HMENU)ID_GAME_START,
		hInst,
		NULL
	);
}

void DeleteSelectMode() {
	ShowWindow(hGameTitle, SW_HIDE);
	ShowWindow(hSoloPlayerButton, SW_HIDE);
	ShowWindow(hMultiPlayerButton, SW_HIDE);
}

void DeleteServerInfo() {
	ShowWindow(hGameTitle, SW_HIDE);
	ShowWindow(hIPAddressStatic, SW_HIDE);
	ShowWindow(hPortStatic, SW_HIDE);
	ShowWindow(hIPAddress, SW_HIDE);
	ShowWindow(hPort, SW_HIDE);
	ShowWindow(hServerBackButton, SW_HIDE);
	ShowWindow(hServerNextButton, SW_HIDE);
}

void DeleteRoomType() {
	ShowWindow(hRoomTypeStatoc, SW_HIDE);
	ShowWindow(hCreateRoomButton, SW_HIDE);
	ShowWindow(hJoinRoomButton, SW_HIDE);
}

void DeleteRoomCode() {
	ShowWindow(hRoomCodeStatic, SW_HIDE);
	ShowWindow(hCodeStatic, SW_HIDE);
	ShowWindow(hCode, SW_HIDE);
	ShowWindow(hCodeBackButton, SW_HIDE);
	ShowWindow(hCodeNextButton, SW_HIDE);
}

void DeleteGameStart() {
	ShowWindow(hGameStartStatic, SW_HIDE);
	ShowWindow(hStartRoomCodeStatic, SW_HIDE);
	ShowWindow(hGameStartButton, SW_HIDE);
}