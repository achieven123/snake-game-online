#pragma once

#include <Windows.h>

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
	
	/*
	* @brief map 초기화 함수
	* @param posX, posY map 생성 위치
	* @details map 초기 위치를 정하고, 초기화
	*/
	void initMap(int posX, int posY);
	
	/*
	* @brief map 그리기 함수
	* @param hdc 디바이스 컨텍스트에 대한 핸들
	* @details 2차원 배열 안에 있는 값에 대해 그림 그리기
	*/
	void drawMap(HDC hdc);
};