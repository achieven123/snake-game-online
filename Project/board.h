#pragma once

#include <Windows.h>
#include <stdlib.h>

class Board {
public:
	static const int HEIGHT = 22;
	static const int WIDTH = 22;
	static const int LENGTH = 20;

	static const int GROUND = 0;
	static const int WALL = 1;
	static const int FOOD = 2;
	static const int HEAD = 3;
	static const int BODY = 4;

private:
	int posX;
	int posY;
	int map[HEIGHT][WIDTH];

public:
	int getPosX();
	int getPosY();

	int(&getMap())[HEIGHT][WIDTH];
	void generateFood(int count);
	void initMap(int posX, int posY);
	void drawMap(HDC hdc);
};