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
	* @brief ���� ���� �Լ�
	* @param host ������ ������ IP �ּ�
	* @param port ������ ������ Port ��ȣ
	* @return ���� ���� �� True ��ȯ
	* @details ip�ּҿ� port ��ȣ�� ������ ����
	*/
	bool connectServer(HWND hwnd, const char* host, const char* port);

	/*
	* @brief Room ���� �Լ�
	* @details ������ create �޽����� ����
	*/
	void createRoom();
	
	/*
	* @brief Room ���� �Լ�
	* @param code ������ ���� Room Code
	* @details ������ join �޽����� ����
	*/
	bool joinRoom(HWND hwnd, const string code);

	/*
	* @brief ���� ���� �Լ�
	* @details ������ start �޽����� ����
	*/
	void startGame();

	/*
	* @brief ���� ���� �Լ�
	* @details ������ end �޽����� ����
	*/
	void endGame();


	/*
	* @brief Game �ʱ�ȭ �Լ�
	* @details ������ ��ġ�� 10���� ���� ����, map ���� ��ġ ����
	*/
	void initGame();


	void drawGame(HDC hdc);
	void setDirect(int directKey);
	void moveSnake();
};