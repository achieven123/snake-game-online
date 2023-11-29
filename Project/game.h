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

	static const int HOST = 0;
	static const int GUEST = 1;

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
	Snake snake1, snake2;
	Board board1, board2;
	Client client;
	Tools tools;
	
	string msg;
	string code;
	
	int mode;
	int type;
	int state;

	bool isWin = true;

public:
	string getCode();
	int getMode();
	int getType();
	int getState();
	bool getIsWin();

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


	void drawGame(HDC hdc);
	void setDirect(int directKey);
	void moveSnake();
};