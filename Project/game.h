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
	* @brief ���� ���� �Լ�
	* @param host ������ ������ IP �ּ�
	* @param port ������ ������ Port ��ȣ
	* @return ���� ���� �� True ��ȯ
	* @details ip�ּҿ� port ��ȣ�� ������ ����
	*/
	bool connectServer(HWND hwnd, const char* host, const char* port);

	void createRoom();

	bool joinRoom(HWND hwnd, const string code);

	/*
	* @brief Game �ʱ�ȭ �Լ�
	* @details ������ ��ġ�� 10���� ���� ����, map ���� ��ġ ����
	*/
	void initGame();

	void startGame();
	void endGame();

	void drawGame(HDC hdc);
	void setDirect(int directKey);
	bool moveSnake(HDC hdc, bool multi);
};