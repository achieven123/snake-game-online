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

#define ID_READY 0
#define ID_START 1
#define ID_END 2
#define ID_GAME 3

using namespace std;

class Game {
private:
	Snake snake1, snake2;
	Board board1, board2;
	Client client;
	Tools tools;
	
	string msg;
	string code;
	int player;
	int state = ID_READY;

public:
	string getCode();
	int getPlayer();
	int getState();

	void setState(int state);

	/*
	* @brief 서버 연결 함수
	* @param host 연결할 서버의 IP 주소
	* @param port 연결할 서버의 Port 번호
	* @return 연결 성공 시 True 반환
	* @details ip주소와 port 번호로 서버와 연결
	*/
	bool connectServer(HWND hwnd, const char* host, const char* port);

	void createRoom();

	bool joinRoom(HWND hwnd, const string code);

	/*
	* @brief Game 초기화 함수
	* @details 랜덤한 위치에 10개의 음식 생성, map 생성 위치 지정
	*/
	void initGame();

	void startGame();
	void endGame();

	void drawGame(HDC hdc);
	void setDirect(int directKey);
	bool moveSnake(HDC hdc, bool multi);
};