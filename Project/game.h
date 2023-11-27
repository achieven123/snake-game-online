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

#define SOLO_PLAYER 1
#define MULTI_PLAYER 2

using namespace std;

class Game {
private:
	Snake snake1, snake2;
	Board board1, board2;
	Client client;

public:
	bool initGame(HWND hwnd, const char* _host, const char* _port);
	void createRoom();
	bool joinRoom(HWND hwnd, string code);
	void endGame();
	void startGame(bool multi);

	void setDirect(int directKey);
	bool moveSnake(HDC hdc, bool multi);
	string getSnakeCode();
	bool IsTwoPlayer();
};