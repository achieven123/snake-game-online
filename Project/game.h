#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "snake.h"
#include "board.h"
#include "client.h"
#include "tools.h"

using namespace std;

class Game {
public:
	static const int SOLO = 0;
	static const int MULTI = 1;

	// Multi mode에서 Host인지 Guest 인지 판단하는 상수
	static const int HOST = 0;
	static const int GUEST = 1;

	// 현재 화면에 그릴 윈도우의 상태를 나타내는 상수
	static const int MAIN_MENU = 0;
	static const int SERVER_INFO = 1;
	static const int SELECT_TYPE = 2;
	static const int HOST_WAIT_GUEST = 3;
	static const int HOST_WAIT_GAME_START = 4;
	static const int GUEST_WAIT_CODE_INPUT = 5;
	static const int GUEST_WAIT_HOST_GAME_START = 6;
	static const int GAME_START = 7;
	static const int GAME_IN_PROGRESS = 8;
	static const int GAME_END = 9;

private:
	Snake snake1, snake2; // 나의 Snake, 상대 Snake
	Board board1, board2; // 나의 Board, 상대 Board
	Client client; // 서버와 통신을 위함 Client 클래스
	Tools tools; // 자주 사용하는 툴 함수 클래스
	
	string msg; // 서버와 통신을 위한 msg
	string code; // 방 코드
	
	int mode; // Solo/Multi mode 판별을 위한 변수
	int type; // Multi mode에서 Host인지 Guest인지 판단
	int state; // 현재 화면에 그릴 윈도우의 상태를 나타내는 상수를 저장

	bool isWin = true; // Multi mode에서 이겼는지 판단하는 변수

public:
	// private 변수 getter
	string getCode();
	int getMode();
	int getType();
	int getState();
	bool getIsWin();

	// private 변수 setter
	void setMode(int mode);
	void setType(int type);
	void setState(int state);
	void setIsWin(bool isWin);

	/*
	* @brief 서버 연결 함수
	* @param host 연결할 서버의 IP 주소
	* @param port 연결할 서버의 Port 번호
	* @return 연결 성공 시 True 반환
	* @details ip주소와 port 번호로 서버와 연결
	*/
	bool connectServer(HWND hwnd, const char* host, const char* port);

	/*
	* @brief Room 생성 함수
	* @details 서버에 create 메시지를 보냄
	*/
	void createRoom();
	
	/*
	* @brief Room 참가 함수
	* @param code 서버에 보낼 Room Code
	* @details 서버에 join 메시지를 보냄
	*/
	bool joinRoom(HWND hwnd, const string code);

	/*
	* @brief 게임 시작 함수
	* @details 서버에 start 메시지를 보냄
	*/
	void startGame();

	/*
	* @brief 게임 종료 함수
	* @details 서버에 end 메시지를 보냄
	*/
	void endGame();


	/*
	* @brief Game 초기화 함수
	* @details 랜덤한 위치에 10개의 음식 생성, map 생성 위치 지정
	*/
	void initGame();

	/*
	* @brief 게임 map과 정보들을 그리는 함수
	* @details solo/Multi mode에 따라서 map을 한 개 그리거나 두 개 그림
	*/
	void drawGame(HDC hdc);

	/*
	* @brief 입력한 방향키대로 뱀의 방향을 조정
	* @details 입력한 방향키를 Snake 클래스로 전달하여 방향을 정하도록 함
	*/
	void setDirect(int directKey);

	/*
	* @brief Snake의 방향으로 뱀을 한 칸 이동
	* @details
	* 다음 위치에서의 충돌 판정을 Snake 클래스의 moveSnake를 통해 확인
	* Multi mode에서는 snake정보를 서버로 보냄
	*/
	void moveSnake();
};