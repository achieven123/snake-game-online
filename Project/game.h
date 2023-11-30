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

	// Multi mode���� Host���� Guest ���� �Ǵ��ϴ� ���
	static const int HOST = 0;
	static const int GUEST = 1;

	// ���� ȭ�鿡 �׸� �������� ���¸� ��Ÿ���� ���
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
	Snake snake1, snake2; // ���� Snake, ��� Snake
	Board board1, board2; // ���� Board, ��� Board
	Client client; // ������ ����� ���� Client Ŭ����
	Tools tools; // ���� ����ϴ� �� �Լ� Ŭ����
	
	string msg; // ������ ����� ���� msg
	string code; // �� �ڵ�
	
	int mode; // Solo/Multi mode �Ǻ��� ���� ����
	int type; // Multi mode���� Host���� Guest���� �Ǵ�
	int state; // ���� ȭ�鿡 �׸� �������� ���¸� ��Ÿ���� ����� ����

	bool isWin = true; // Multi mode���� �̰���� �Ǵ��ϴ� ����

public:
	// private ���� getter
	string getCode();
	int getMode();
	int getType();
	int getState();
	bool getIsWin();

	// private ���� setter
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

	/*
	* @brief ���� map�� �������� �׸��� �Լ�
	* @details solo/Multi mode�� ���� map�� �� �� �׸��ų� �� �� �׸�
	*/
	void drawGame(HDC hdc);

	/*
	* @brief �Է��� ����Ű��� ���� ������ ����
	* @details �Է��� ����Ű�� Snake Ŭ������ �����Ͽ� ������ ���ϵ��� ��
	*/
	void setDirect(int directKey);

	/*
	* @brief Snake�� �������� ���� �� ĭ �̵�
	* @details
	* ���� ��ġ������ �浹 ������ Snake Ŭ������ moveSnake�� ���� Ȯ��
	* Multi mode������ snake������ ������ ����
	*/
	void moveSnake();
};