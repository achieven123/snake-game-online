#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include "board.h"

using namespace std;

class Snake {
private:
	string msg;
	string code;
	int score;
	int length;
	int dx;
	int dy;
	vector<pair<int, int>> food;
	vector<pair<int, int>> location;

public:
	Snake();
	
	string getMsg();
	string getCode();
	int getScore();
	int getDx();
	int getDy();
	int getLength();
	vector<pair<int, int>> getFood();
	vector<pair<int, int>> getLocation();

	void setMsg(string msg);
	void setCode(string code);
	void setScore(int score);
	void setDx(int dx);
	void setDy(int dy);
	void setLength(int length);
	void setFood(vector<pair<int, int>> food);
	void setLocation(vector<pair<int, int>> location);

	bool isOverlap(int x, int y);
	void setDirect(Snake& snake, int directKey);
	void generateFood(int count, int(&map)[Board::HEIGHT][Board::WIDTH]);
	bool moveSnake(Snake& snake, Board& board);
	void setMapValue(Snake& snake, Board& board);
	void printSnake();
};