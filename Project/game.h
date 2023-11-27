#pragma once

#define HOST "113.198.230.13"
#define PORT "8080"

#include <Windows.h>
#include <string>
#include <vector>

#include "snake.h"
#include "board.h"
#include "client.h"
#include "tools.h"

using namespace std;

class Game {
private:
	Snake snake1, snake2;
	Board board1, board2;
	Client client;

public:
	void initGame(HWND hwnd);
	void startGame();
	void endGame();
	void setDirect(int directKey);
	bool moveSnake(HDC hdc);
};