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

// 윈도우 클래스 이름을 상수 문자열로 정의
LPCTSTR mainMenuClass = TEXT("Main Menu Class");
LPCTSTR soloPlayerClass = TEXT("Solo Player Class");
LPCTSTR multiPlayerClass = TEXT("Multi Player Class");

// 윈도우 프로시저 콜백 함수들
LRESULT CALLBACK WinProcMainMenu(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProcSoloPlayerGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProcMultiPlayerGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// 윈도우에 그리기 관련 함수들
void DrawSetting(HWND hwnd);
void DrawWindow(HWND hwnd);
void DrawMainMenu(HWND hwnd);
void DrawInputServerInfo(HWND hwnd);
void DrawSelectType(HWND hwnd);
void DrawHostWaitGuest(HWND hwnd);
void DrawHostWaitStartGame(HWND hwnd);
void DrawGuestWaitInputCode(HWND hwnd);
void DrawGuestWaitHostStartGame(HWND hwnd);

// 모든 윈도우를 삭제하는 함수
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

	case WM_COMMAND: // 다양한 컨트롤들의 ID를 통한 명령 처리
		switch (LOWORD(wParam)) {
		case ID_SOLO_PLAYER_BUTTON:
			// 솔로 플레이어 버튼 클릭 처리
			game.setMode(Game::SOLO); // 게임 모드를 솔로로 설정
			game.setState(Game::HOST_WAIT_GAME_START); // 게임 상태를 호스트 대기 상태로 설정
			DrawWindow(hwnd); // 화면 그리기 함수 호출
			break;

		case ID_MULTI_PLAYER_BUTTON:
			// 멀티 플레이어 버튼 클릭 처리
			game.setMode(Game::MULTI); // 게임 모드를 멀티로 설정
			game.setState(Game::SERVER_INFO); // 게임 상태를 서버 정보 입력 상태로 설정
			DrawWindow(hwnd); // 화면 그리기 함수 호출
			SetTimer(hwnd, Game::MULTI, 500, NULL); // 타이머 설정
			break;

		case ID_INPUT_SERVER_INFO_BACK_BUTTON:
			// 서버 정보 입력 화면의 뒤로 가기 버튼 클릭 처리
			game.setState(Game::MAIN_MENU); // 게임 상태를 메인 메뉴로 설정
			DrawMainMenu(hwnd); // 메인 메뉴 화면 그리기 함수 호출
			break;

		case ID_INPUT_SERVER_INFO_NEXT_BUTTON:
			// 서버 정보 입력 화면의 다음 버튼 클릭 처리
			WCHAR _host[256];
			WCHAR _port[6];

			char host[256];
			char port[8];

			GetWindowText(hIPAddress, _host, 256);
			GetWindowText(hPort, _port, 6);

			// 유니코드 문자열을 UTF-8 형식의 문자열로 변환
			WideCharToMultiByte(CP_UTF8, 0, _host, -1, host, 256, NULL, NULL);
			WideCharToMultiByte(CP_UTF8, 0, _port, -1, port, 8, NULL, NULL);

			bool connectSuccessful;
			connectSuccessful = game.connectServer(hwnd, host, port); // 서버 연결 시도

			if (connectSuccessful) {
				game.setState(Game::SELECT_TYPE); // 게임 상태를 유형 선택으로 설정
				DrawWindow(hwnd); // 화면 그리기 함수 호출
			}
			break;

		case ID_CREATE_ROOM:
			// 방 만들기 버튼 클릭 처리
			game.createRoom(); // 방 생성
			game.setType(Game::HOST); // 게임 유형을 호스트로 설정
			game.setState(Game::HOST_WAIT_GUEST); // 게임 상태를 게스트 대기 상태로 설정
			DrawWindow(hwnd); // 화면 그리기 함수 호출
			break;

		case ID_JOIN_ROOM:
			// 방 참가 버튼 클릭 처리
			game.setType(Game::GUEST); // 게임 유형을 게스트로 설정
			game.setState(Game::GUEST_WAIT_CODE_INPUT); // 게임 상태를 코드 입력 대기 상태로 설정
			DrawWindow(hwnd); // 화면 그리기 함수 호출
			break;

		case ID_INPUT_ROOM_CODE_BACK_BUTTON:
			// 방 코드 입력 화면의 뒤로 가기 버튼 클릭 처리
			game.setState(Game::SELECT_TYPE); // 게임 상태를 유형 선택으로 설정
			DrawWindow(hwnd); // 화면 그리기 함수 호출
			break;

		case ID_INPUT_ROOM_CODE_NEXT_BUTTON:
			// 방 코드 입력 화면의 다음 버튼 클릭 처리
			WCHAR _code[7];

			char code[7];

			GetWindowText(hCode, _code, 7);

			WideCharToMultiByte(CP_UTF8, 0, _code, -1, code, 7, NULL, NULL);

			bool joinSuccessful;
			joinSuccessful = game.joinRoom(hwnd, code); // 방 참가 시도

			if (joinSuccessful) {
				game.setState(Game::GUEST_WAIT_HOST_GAME_START); // 게스트 대기 상태로 설정
				DrawWindow(hwnd); // 화면 그리기 함수 호출
			}
			break;

		case ID_GAME_START:
			// 게임 시작 버튼 클릭 처리
			game.setState(Game::GAME_START); // 게임 상태를 게임 시작으로 설정
			DrawWindow(hwnd); // 화면 그리기 함수 호출
			break;

		case ID_GAME_START_BACK_BUTTON:
			// 게임 시작 화면의 뒤로 가기 버튼 클릭 처리
			if (game.getMode() == Game::SOLO) game.setState(Game::MAIN_MENU); // 솔로 모드일 때 메인 메뉴로 설정
			DrawWindow(hwnd); // 화면 그리기 함수 호출
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
		if (MessageBox(hwnd, _T("게임을 종료 하시겠습니까?"), _T("종료 확인"), MB_YESNO) == IDNO) {
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
		// 윈도우 생성 시 초기화 등의 작업 수행
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps); // 페인트 시작
		game.drawGame(hdc); // 게임 화면 그리기

		if (game.getState() == Game::GAME_START) {
			game.initGame(); // 게임 초기화
			game.setState(Game::GAME_IN_PROGRESS); // 게임 진행 중 상태로 변경
			SetTimer(hwnd, 1, 100, NULL); // 타이머 설정
		}
		else if (game.getState() == Game::GAME_IN_PROGRESS) {
			game.moveSnake(); // 뱀 이동
		}

		// 게임 승리 여부 확인
		if (game.getIsWin() == false) {
			game.setIsWin(true); // 승리 상태 설정
			KillTimer(hwnd, 1); // 타이머 종료
			MessageBox(hwnd, L"게임 종료!", L"알림", MB_OK | MB_ICONWARNING); // 메시지 박스 출력
			ShowWindow(hMainMenu, SW_SHOWNORMAL); // 메인 메뉴 윈도우 표시
			ShowWindow(hSoloPlayerGame, SW_HIDE); // 현재 솔로 플레이어 게임 윈도우 숨김
			game.setState(Game::HOST_WAIT_GAME_START); // 게임 상태를 호스트 대기 상태로 설정
		}

		EndPaint(hwnd, &ps); // 페인트 종료
		break;

	case WM_KEYDOWN:
		// 키 입력 처리
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			game.setDirect(wParam); // 뱀 이동 방향 설정
			break;
		}
		break;

	case WM_TIMER:
		// 타이머 메시지 발생 시 윈도우 영역을 갱신하여 다시 그리기
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_CLOSE:
		// 윈도우 닫기 버튼 동작 처리
		if (MessageBox(hwnd, _T("게임을 종료 하시겠습니까?"), _T("종료 확인"), MB_YESNO) == IDNO) {
			return 0;
		}
		break;

	case WM_DESTROY:
		KillTimer(hwnd, 1); // 타이머 종료
		PostQuitMessage(0); // 프로그램 종료 메시지 전달
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
LRESULT CALLBACK WinProcMultiPlayerGame(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_CREATE:
		// 윈도우 생성 시 초기화 등의 작업 수행
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps); // 페인트 시작
		game.drawGame(hdc); // 게임 화면 그리기
		cout << game.getState() << endl; // 현재 게임 상태 출력

		if (game.getState() == Game::GAME_START) {
			game.setState(Game::GAME_IN_PROGRESS); // 게임 진행 중 상태로 변경
			game.initGame(); // 게임 초기화
			SetTimer(hwnd, 1, 100, NULL); // 타이머 설정
			Sleep(1000); // 1초 지연
		}
		else if (game.getState() == Game::GAME_IN_PROGRESS) {
			game.moveSnake(); // 뱀 이동
		}
		else if (game.getState() == Game::GAME_END) {
			KillTimer(hwnd, 1); // 타이머 종료

			// 게임 결과에 따라 메시지 박스 출력
			if (game.getIsWin()) {
				MessageBox(hwnd, L"이겼습니다!", L"알림", MB_OK | MB_ICONWARNING);
			}
			else {
				MessageBox(hwnd, L"졌습니다!", L"알림", MB_OK | MB_ICONWARNING);
			}

			game.setIsWin(true);

			// 게임 유형에 따라 상태 설정 및 윈도우 표시 설정
			if (game.getType() == Game::HOST) game.setState(Game::HOST_WAIT_GAME_START);
			if (game.getType() == Game::GUEST) game.setState(Game::GUEST_WAIT_HOST_GAME_START);

			ShowWindow(hMainMenu, SW_SHOWNORMAL); // 메인 메뉴 윈도우 표시
			ShowWindow(hMultiPlayerGame, SW_HIDE); // 현재 멀티 플레이어 게임 윈도우 숨김
		}

		EndPaint(hwnd, &ps); // 페인트 종료
		break;

	case WM_KEYDOWN:
		// 키 입력 처리
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			game.setDirect(wParam); // 뱀 이동 방향 설정
			break;
		}
		break;

	case WM_TIMER:
		// 타이머 메시지 발생 시 윈도우 영역을 갱신하여 다시 그리기
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_CLOSE:
		// 윈도우 닫기 버튼 동작 처리
		if (MessageBox(hwnd, _T("게임을 종료 하시겠습니까?"), _T("종료 확인"), MB_YESNO) == IDNO) {
			return 0;
		}
		break;

	case WM_DESTROY:
		game.endGame(); // 게임 종료
		KillTimer(hwnd, 1); // 타이머 종료
		PostQuitMessage(0); // 프로그램 종료 메시지 전달
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
	DeleteAllWindows(); // 이전에 그려진 모든 윈도우 삭제

	// 솔로 플레이 모드인 경우
	if (game.getMode() == Game::SOLO) {
		switch (game.getState()) {
		case Game::MAIN_MENU:
			DrawMainMenu(hwnd); // 메인 메뉴 그리기
			break;

		case Game::HOST_WAIT_GAME_START:
			DrawHostWaitStartGame(hwnd); // 게임 시작 대기 화면 그리기
			break;

		case Game::GAME_START:
			ShowWindow(hMainMenu, SW_HIDE); // 메인 메뉴 숨기기
			ShowWindow(hSoloPlayerGame, SW_SHOWNORMAL); // 솔로 플레이어 게임 윈도우 표시
			break;
		}
	}
	// 멀티 플레이 모드인 경우
	else if (game.getMode() == Game::MULTI) {
		switch (game.getState()) {
		case Game::MAIN_MENU:
			DrawMainMenu(hwnd); // 메인 메뉴 그리기
			break;

		case Game::SERVER_INFO:
			DrawInputServerInfo(hwnd); // 서버 정보 입력 화면 그리기
			break;

		case Game::SELECT_TYPE:
			DrawSelectType(hwnd); // 플레이어 유형 선택 화면 그리기
			break;

		case Game::HOST_WAIT_GUEST:
			Sleep(300); // 300ms 대기
			DrawHostWaitGuest(hwnd); // 게스트 대기 화면 그리기
			break;

		case Game::HOST_WAIT_GAME_START:
			DrawHostWaitStartGame(hwnd); // 게임 시작 대기 화면 그리기
			break;

		case Game::GUEST_WAIT_CODE_INPUT:
			DrawGuestWaitInputCode(hwnd); // 코드 입력 대기 화면 그리기
			break;

		case Game::GUEST_WAIT_HOST_GAME_START:
			DrawGuestWaitHostStartGame(hwnd); // 호스트 게임 시작 대기 화면 그리기
			break;

		case Game::GAME_START:
			ShowWindow(hMainMenu, SW_HIDE); // 메인 메뉴 숨기기
			ShowWindow(hMultiPlayerGame, SW_SHOWNORMAL); // 멀티 플레이어 게임 윈도우 표시
			game.startGame(); // 게임 시작
			Sleep(100); // 100ms 대기
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

	hInputServerInfoBackButton = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"< 뒤로",
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
		L"다음 >",
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
		L"참여 방법 선택",
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
		L"상대를 기다리는 중...",
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
		L"게임을 시작하겠습니까?",
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
			L"< 뒤로",
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
		L"Room Code 입력",
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
		L"< 뒤로",
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
		L"다음 >",
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
		L"호스트가 게임을 시작하길 기다리는 중...",
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