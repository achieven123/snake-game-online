//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")

#include <windows.h>
#include <tchar.h>
#include "game.h"
#include "tools.h"

#define ID_SOLO_PLAYER_BUTTON 100
#define ID_MULTI_PLAYER_BUTTON 101
#define ID_INPUT_SERVER_INFO_BACK_BUTTON 200
#define ID_INPUT_SERVER_INFO_NEXT_BUTTON 201
#define ID_CREATE_ROOM 300
#define ID_JOIN_ROOM 301
#define ID_INPUT_ROOM_CODE_BACK_BUTTON 400
#define ID_INPUT_ROOM_CODE_NEXT_BUTTON 401
#define ID_GAME_START 500
#define ID_GAME_START_BACK_BUTTON 501

using namespace std;

Game game;
Tools tools;

HFONT hFont;
RECT rcClient;
int width, height;

HINSTANCE hInst;
HWND hMainMenu, hSoloPlayerGame, hMultiPlayerGame;
HWND hGameTitle, hSoloPlayerButton, hMultiPlayerButton;
HWND hIPAddressStatic, hPortStatic, hIPAddress, hPort, hInputServerInfoBackButton, hInputServerInfoNextButton;
HWND hSelectTypeStatic, hCreateRoomButton, hJoinRoomButton;
HWND hHostWaitGuest, hRoomCodeStatic;
HWND hHostGameStartStatic, hHostGameStartButton, hHostGameStartBackButton;
HWND hGuestWaitCodeInputStatic, hCodeStatic, hCode, hGuestWaitCodeInputBackButton, hGuestWaitCodeInputNextButton;
HWND hGuestWaitHostGameStartStatic;

// ������ Ŭ���� �̸��� ��� ���ڿ��� ����
LPCTSTR mainMenuClass = TEXT("Main Menu Class");
LPCTSTR soloPlayerClass = TEXT("Solo Player Class");
LPCTSTR multiPlayerClass = TEXT("Multi Player Class");

// ������ ���ν��� �ݹ� �Լ���
LRESULT CALLBACK WinProcMainMenu(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProcSoloPlayerGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProcMultiPlayerGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// �����쿡 �׸��� ���� �Լ���
void DrawSetting(HWND hwnd);
void DrawWindow(HWND hwnd);
void DrawMainMenu(HWND hwnd);
void DrawInputServerInfo(HWND hwnd);
void DrawSelectType(HWND hwnd);
void DrawHostWaitGuest(HWND hwnd);
void DrawHostWaitStartGame(HWND hwnd);
void DrawGuestWaitInputCode(HWND hwnd);
void DrawGuestWaitHostStartGame(HWND hwnd);

// ��� �����츦 �����ϴ� �Լ�
void DeleteAllWindows();

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

	WndClass.lpszClassName = mainMenuClass;
	WndClass.lpfnWndProc = WinProcMainMenu;
	RegisterClass(&WndClass);

	WndClass.lpszClassName = soloPlayerClass;
	WndClass.lpfnWndProc = WinProcSoloPlayerGame;
	RegisterClass(&WndClass);

	WndClass.lpszClassName = multiPlayerClass;
	WndClass.lpfnWndProc = WinProcMultiPlayerGame;
	RegisterClass(&WndClass);

	hMainMenu = CreateWindowEx(
		0,
		mainMenuClass,
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

	ShowWindow(hMainMenu, nCmdShow);

	hSoloPlayerGame = CreateWindowEx(
		0,
		soloPlayerClass,
		L"Snake Game",
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		455, 530,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hSoloPlayerGame, SW_HIDE);

	hMultiPlayerGame = CreateWindowEx(
		0,
		multiPlayerClass,
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

	ShowWindow(hMultiPlayerGame, SW_HIDE);

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
	case WM_PAINT:
		DrawSetting(hwnd);
		DrawWindow(hwnd);
		break;

	case WM_COMMAND: // �پ��� ��Ʈ�ѵ��� ID�� ���� ��� ó��
		switch (LOWORD(wParam)) {
		case ID_SOLO_PLAYER_BUTTON:
			// �ַ� �÷��̾� ��ư Ŭ�� ó��
			game.setMode(Game::SOLO); // ���� ��带 �ַη� ����
			game.setState(Game::HOST_WAIT_GAME_START); // ���� ���¸� ȣ��Ʈ ��� ���·� ����
			DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			break;

		case ID_MULTI_PLAYER_BUTTON:
			// ��Ƽ �÷��̾� ��ư Ŭ�� ó��
			game.setMode(Game::MULTI); // ���� ��带 ��Ƽ�� ����
			game.setState(Game::SERVER_INFO); // ���� ���¸� ���� ���� �Է� ���·� ����
			DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			SetTimer(hwnd, Game::MULTI, 500, NULL); // Ÿ�̸� ����
			break;

		case ID_INPUT_SERVER_INFO_BACK_BUTTON:
			// ���� ���� �Է� ȭ���� �ڷ� ���� ��ư Ŭ�� ó��
			game.setState(Game::MAIN_MENU); // ���� ���¸� ���� �޴��� ����
			DrawMainMenu(hwnd); // ���� �޴� ȭ�� �׸��� �Լ� ȣ��
			break;

		case ID_INPUT_SERVER_INFO_NEXT_BUTTON:
			// ���� ���� �Է� ȭ���� ���� ��ư Ŭ�� ó��
			WCHAR _host[256];
			WCHAR _port[6];

			char host[256];
			char port[8];

			GetWindowText(hIPAddress, _host, 256);
			GetWindowText(hPort, _port, 6);

			// �����ڵ� ���ڿ��� UTF-8 ������ ���ڿ��� ��ȯ
			WideCharToMultiByte(CP_UTF8, 0, _host, -1, host, 256, NULL, NULL);
			WideCharToMultiByte(CP_UTF8, 0, _port, -1, port, 8, NULL, NULL);

			bool connectSuccessful;
			connectSuccessful = game.connectServer(hwnd, host, port); // ���� ���� �õ�

			if (connectSuccessful) {
				game.setState(Game::SELECT_TYPE); // ���� ���¸� ���� �������� ����
				DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			}
			break;

		case ID_CREATE_ROOM:
			// �� ����� ��ư Ŭ�� ó��
			game.createRoom(); // �� ����
			game.setType(Game::HOST); // ���� ������ ȣ��Ʈ�� ����
			game.setState(Game::HOST_WAIT_GUEST); // ���� ���¸� �Խ�Ʈ ��� ���·� ����
			DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			break;

		case ID_JOIN_ROOM:
			// �� ���� ��ư Ŭ�� ó��
			game.setType(Game::GUEST); // ���� ������ �Խ�Ʈ�� ����
			game.setState(Game::GUEST_WAIT_CODE_INPUT); // ���� ���¸� �ڵ� �Է� ��� ���·� ����
			DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			break;

		case ID_INPUT_ROOM_CODE_BACK_BUTTON:
			// �� �ڵ� �Է� ȭ���� �ڷ� ���� ��ư Ŭ�� ó��
			game.setState(Game::SELECT_TYPE); // ���� ���¸� ���� �������� ����
			DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			break;

		case ID_INPUT_ROOM_CODE_NEXT_BUTTON:
			// �� �ڵ� �Է� ȭ���� ���� ��ư Ŭ�� ó��
			WCHAR _code[7];

			char code[7];

			GetWindowText(hCode, _code, 7);

			WideCharToMultiByte(CP_UTF8, 0, _code, -1, code, 7, NULL, NULL);

			bool joinSuccessful;
			joinSuccessful = game.joinRoom(hwnd, code); // �� ���� �õ�

			if (joinSuccessful) {
				game.setState(Game::GUEST_WAIT_HOST_GAME_START); // �Խ�Ʈ ��� ���·� ����
				DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			}
			break;

		case ID_GAME_START:
			// ���� ���� ��ư Ŭ�� ó��
			game.setState(Game::GAME_START); // ���� ���¸� ���� �������� ����
			DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			break;

		case ID_GAME_START_BACK_BUTTON:
			// ���� ���� ȭ���� �ڷ� ���� ��ư Ŭ�� ó��
			if (game.getMode() == Game::SOLO) game.setState(Game::MAIN_MENU); // �ַ� ����� �� ���� �޴��� ����
			DrawWindow(hwnd); // ȭ�� �׸��� �Լ� ȣ��
			break;
		}

		break;

	case WM_CTLCOLORSTATIC:
		HDC hdcStatic;
		hdcStatic = (HDC)wParam;
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)GetStockObject(NULL_BRUSH);

	case WM_TIMER:
		if (wParam == Game::MULTI) {
			cout << game.getState() << endl;
			if (game.getState() == Game::HOST_WAIT_GAME_START || game.getState() == Game::GAME_START) {
				DrawWindow(hwnd);
			}
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hwnd, _T("������ ���� �Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDNO) {
			return 0;
		}

		break;

	case WM_DESTROY:
		game.endGame();
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK WinProcSoloPlayerGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_CREATE:
		// ������ ���� �� �ʱ�ȭ ���� �۾� ����
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps); // ����Ʈ ����
		game.drawGame(hdc); // ���� ȭ�� �׸���

		if (game.getState() == Game::GAME_START) {
			game.initGame(); // ���� �ʱ�ȭ
			game.setState(Game::GAME_IN_PROGRESS); // ���� ���� �� ���·� ����
			SetTimer(hwnd, 1, 100, NULL); // Ÿ�̸� ����
		}
		else if (game.getState() == Game::GAME_IN_PROGRESS) {
			game.moveSnake(); // �� �̵�
		}

		// ���� �¸� ���� Ȯ��
		if (game.getIsWin() == false) {
			game.setIsWin(true); // �¸� ���� ����
			KillTimer(hwnd, 1); // Ÿ�̸� ����
			MessageBox(hwnd, L"���� ����!", L"�˸�", MB_OK | MB_ICONWARNING); // �޽��� �ڽ� ���
			ShowWindow(hMainMenu, SW_SHOWNORMAL); // ���� �޴� ������ ǥ��
			ShowWindow(hSoloPlayerGame, SW_HIDE); // ���� �ַ� �÷��̾� ���� ������ ����
			game.setState(Game::HOST_WAIT_GAME_START); // ���� ���¸� ȣ��Ʈ ��� ���·� ����
		}

		EndPaint(hwnd, &ps); // ����Ʈ ����
		break;

	case WM_KEYDOWN:
		// Ű �Է� ó��
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			game.setDirect(wParam); // �� �̵� ���� ����
			break;
		}
		break;

	case WM_TIMER:
		// Ÿ�̸� �޽��� �߻� �� ������ ������ �����Ͽ� �ٽ� �׸���
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_CLOSE:
		// ������ �ݱ� ��ư ���� ó��
		if (MessageBox(hwnd, _T("������ ���� �Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDNO) {
			return 0;
		}
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1); // Ÿ�̸� ����
		PostQuitMessage(0); // ���α׷� ���� �޽��� ����
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
LRESULT CALLBACK WinProcMultiPlayerGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_CREATE:
		// ������ ���� �� �ʱ�ȭ ���� �۾� ����
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps); // ����Ʈ ����
		game.drawGame(hdc); // ���� ȭ�� �׸���
		cout << game.getState() << endl; // ���� ���� ���� ���

		if (game.getState() == Game::GAME_START) {
			game.setState(Game::GAME_IN_PROGRESS); // ���� ���� �� ���·� ����
			game.initGame(); // ���� �ʱ�ȭ
			SetTimer(hwnd, 1, 100, NULL); // Ÿ�̸� ����
			Sleep(1000); // 1�� ����
		}
		else if (game.getState() == Game::GAME_IN_PROGRESS) {
			game.moveSnake(); // �� �̵�
		}
		else if (game.getState() == Game::GAME_END) {
			KillTimer(hwnd, 1); // Ÿ�̸� ����

			// ���� ����� ���� �޽��� �ڽ� ���
			if (game.getIsWin()) {
				MessageBox(hwnd, L"�̰���ϴ�!", L"�˸�", MB_OK | MB_ICONWARNING);
			}
			else {
				MessageBox(hwnd, L"�����ϴ�!", L"�˸�", MB_OK | MB_ICONWARNING);
			}

			game.setIsWin(true);

			// ���� ������ ���� ���� ���� �� ������ ǥ�� ����
			if (game.getType() == Game::HOST) game.setState(Game::HOST_WAIT_GAME_START);
			if (game.getType() == Game::GUEST) game.setState(Game::GUEST_WAIT_HOST_GAME_START);

			ShowWindow(hMainMenu, SW_SHOWNORMAL); // ���� �޴� ������ ǥ��
			ShowWindow(hMultiPlayerGame, SW_HIDE); // ���� ��Ƽ �÷��̾� ���� ������ ����
		}

		EndPaint(hwnd, &ps); // ����Ʈ ����
		break;

	case WM_KEYDOWN:
		// Ű �Է� ó��
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			game.setDirect(wParam); // �� �̵� ���� ����
			break;
		}
		break;

	case WM_TIMER:
		// Ÿ�̸� �޽��� �߻� �� ������ ������ �����Ͽ� �ٽ� �׸���
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_CLOSE:
		// ������ �ݱ� ��ư ���� ó��
		if (MessageBox(hwnd, _T("������ ���� �Ͻðڽ��ϱ�?"), _T("���� Ȯ��"), MB_YESNO) == IDNO) {
			return 0;
		}
		break;

	case WM_DESTROY:
		game.endGame(); // ���� ����
		KillTimer(hwnd, 1); // Ÿ�̸� ����
		PostQuitMessage(0); // ���α׷� ���� �޽��� ����
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

void DrawSetting(HWND hwnd) {
	GetClientRect(hwnd, &rcClient);
	width = rcClient.right - rcClient.left;
	height = rcClient.bottom - rcClient.top;

	hFont = CreateFont(
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

void DrawWindow(HWND hwnd) {
	DeleteAllWindows(); // ������ �׷��� ��� ������ ����

	// �ַ� �÷��� ����� ���
	if (game.getMode() == Game::SOLO) {
		switch (game.getState()) {
		case Game::MAIN_MENU:
			DrawMainMenu(hwnd); // ���� �޴� �׸���
			break;

		case Game::HOST_WAIT_GAME_START:
			DrawHostWaitStartGame(hwnd); // ���� ���� ��� ȭ�� �׸���
			break;

		case Game::GAME_START:
			ShowWindow(hMainMenu, SW_HIDE); // ���� �޴� �����
			ShowWindow(hSoloPlayerGame, SW_SHOWNORMAL); // �ַ� �÷��̾� ���� ������ ǥ��
			break;
		}
	}
	// ��Ƽ �÷��� ����� ���
	else if (game.getMode() == Game::MULTI) {
		switch (game.getState()) {
		case Game::MAIN_MENU:
			DrawMainMenu(hwnd); // ���� �޴� �׸���
			break;

		case Game::SERVER_INFO:
			DrawInputServerInfo(hwnd); // ���� ���� �Է� ȭ�� �׸���
			break;

		case Game::SELECT_TYPE:
			DrawSelectType(hwnd); // �÷��̾� ���� ���� ȭ�� �׸���
			break;

		case Game::HOST_WAIT_GUEST:
			Sleep(300); // 300ms ���
			DrawHostWaitGuest(hwnd); // �Խ�Ʈ ��� ȭ�� �׸���
			break;

		case Game::HOST_WAIT_GAME_START:
			DrawHostWaitStartGame(hwnd); // ���� ���� ��� ȭ�� �׸���
			break;

		case Game::GUEST_WAIT_CODE_INPUT:
			DrawGuestWaitInputCode(hwnd); // �ڵ� �Է� ��� ȭ�� �׸���
			break;

		case Game::GUEST_WAIT_HOST_GAME_START:
			DrawGuestWaitHostStartGame(hwnd); // ȣ��Ʈ ���� ���� ��� ȭ�� �׸���
			break;

		case Game::GAME_START:
			ShowWindow(hMainMenu, SW_HIDE); // ���� �޴� �����
			ShowWindow(hMultiPlayerGame, SW_SHOWNORMAL); // ��Ƽ �÷��̾� ���� ������ ǥ��
			game.startGame(); // ���� ����
			Sleep(100); // 100ms ���
			break;
		}
	}
}

void DrawMainMenu(HWND hwnd) {
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

	SendMessage(hGameTitle, WM_SETFONT, (WPARAM)hFont, TRUE);

	hSoloPlayerButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"Solo Player",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2, (height - 30) / 2 + 50, 100, 30,
		hwnd,
		(HMENU)ID_SOLO_PLAYER_BUTTON,
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
		(HMENU)ID_MULTI_PLAYER_BUTTON,
		hInst,
		NULL
	);
}

void DrawInputServerInfo(HWND hwnd) {
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

	SendMessage(hGameTitle, WM_SETFONT, (WPARAM)hFont, TRUE);

	hIPAddressStatic = CreateWindow(
		L"STATIC",
		L"IP �ּ�",
		WS_CHILD | WS_VISIBLE,
		(width - 50) / 2 - 100, (height - 60) / 2 + 50, 80, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	hPortStatic = CreateWindow(
		L"STATIC",
		L"Port ��ȣ",
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

	hInputServerInfoBackButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"< �ڷ�",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 100, (height - 30) / 2 + 150, 100, 30,
		hwnd,
		(HMENU)ID_INPUT_SERVER_INFO_BACK_BUTTON,
		hInst,
		NULL
	);

	hInputServerInfoNextButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"���� >",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 220, (height - 30) / 2 + 150, 100, 30,
		hwnd,
		(HMENU)ID_INPUT_SERVER_INFO_NEXT_BUTTON,
		hInst,
		NULL
	);
}

void DrawSelectType(HWND hwnd) {
	hSelectTypeStatic = CreateWindow(
		L"STATIC",
		L"���� ��� ����",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 300) / 2, (height - 60) / 2 - 20, 300, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hSelectTypeStatic, WM_SETFONT, (WPARAM)hFont, TRUE);

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

void DrawHostWaitGuest(HWND hwnd) {
	hHostWaitGuest = CreateWindow(
		L"STATIC",
		L"��븦 ��ٸ��� ��...",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 350) / 2, (height - 60) / 2 - 20, 350, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hHostWaitGuest, WM_SETFONT, (WPARAM)hFont, TRUE);
	wstring _code = tools.stringToWString("Room Code: " + game.getCode());
	
	hRoomCodeStatic = CreateWindow(
		L"STATIC",
		_code.c_str(),
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 300) / 2, (height - 60) / 2 + 50, 300, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);
}

void DrawHostWaitStartGame(HWND hwnd) {
	hHostGameStartStatic = CreateWindow(
		L"STATIC",
		L"������ �����ϰڽ��ϱ�?",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 350) / 2, (height - 60) / 2 - 20, 350, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);
	
	SendMessage(hHostGameStartStatic, WM_SETFONT, (WPARAM)hFont, TRUE);

	hHostGameStartButton = CreateWindowEx(
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

	if (game.getMode() == Game::SOLO) {
		hHostGameStartBackButton = CreateWindowEx(
			NULL,
			L"BUTTON",
			L"< �ڷ�",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			(width - 100) / 2 + 220, (height - 30) / 2 + 150, 100, 30,
			hwnd,
			(HMENU)ID_GAME_START_BACK_BUTTON,
			hInst,
			NULL
		);
	}
}

void DrawGuestWaitInputCode(HWND hwnd) {
	hGuestWaitCodeInputStatic = CreateWindow(
		L"STATIC",
		L"Room Code �Է�",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 300) / 2, (height - 60) / 2 - 20, 300, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hGuestWaitCodeInputStatic, WM_SETFONT, (WPARAM)hFont, TRUE);
	
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
		(width - 150) / 2 + 50, (height - 25) / 2 + 27, 150, 25,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	hGuestWaitCodeInputBackButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"< �ڷ�",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 100, (height - 30) / 2 + 150, 100, 30,
		hwnd,
		(HMENU)ID_INPUT_ROOM_CODE_BACK_BUTTON,
		hInst,
		NULL
	);

	hGuestWaitCodeInputNextButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"���� >",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		(width - 100) / 2 + 220, (height - 30) / 2 + 150, 100, 30,
		hwnd,
		(HMENU)ID_INPUT_ROOM_CODE_NEXT_BUTTON,
		hInst,
		NULL
	);
}

void DrawGuestWaitHostStartGame(HWND hwnd) {
	hGuestWaitHostGameStartStatic = CreateWindow(
		L"STATIC",
		L"ȣ��Ʈ�� ������ �����ϱ� ��ٸ��� ��...",
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		(width - 350) / 2, (height - 60) / 2 - 20, 350, 60,
		hwnd,
		NULL,
		hInst,
		NULL
	);

	SendMessage(hGuestWaitHostGameStartStatic, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void DeleteAllWindows() {
	ShowWindow(hGameTitle, SW_HIDE);
	ShowWindow(hSoloPlayerButton, SW_HIDE);
	ShowWindow(hMultiPlayerButton, SW_HIDE);

	ShowWindow(hGameTitle, SW_HIDE);
	ShowWindow(hSelectTypeStatic, SW_HIDE);
	ShowWindow(hCreateRoomButton, SW_HIDE);
	ShowWindow(hJoinRoomButton, SW_HIDE);

	ShowWindow(hHostWaitGuest, SW_HIDE);
	ShowWindow(hRoomCodeStatic, SW_HIDE);

	ShowWindow(hHostGameStartStatic, SW_HIDE);
	ShowWindow(hHostGameStartButton, SW_HIDE);
	ShowWindow(hHostGameStartBackButton, SW_HIDE);

	ShowWindow(hIPAddressStatic, SW_HIDE);
	ShowWindow(hPortStatic, SW_HIDE);
	ShowWindow(hIPAddress, SW_HIDE);
	ShowWindow(hPort, SW_HIDE);
	ShowWindow(hInputServerInfoBackButton, SW_HIDE);
	ShowWindow(hInputServerInfoNextButton, SW_HIDE);

	ShowWindow(hGuestWaitCodeInputStatic, SW_HIDE);
	ShowWindow(hCodeStatic, SW_HIDE);
	ShowWindow(hCode, SW_HIDE);
	ShowWindow(hGuestWaitCodeInputBackButton, SW_HIDE);
	ShowWindow(hGuestWaitCodeInputNextButton, SW_HIDE);

	ShowWindow(hGuestWaitHostGameStartStatic, SW_HIDE);
}