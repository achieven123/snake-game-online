#pragma once

#include <Windows.h>

class Board {
public:
	static const int HEIGHT = 22; // Map의 높이
	static const int WIDTH = 22; // Map의 너비
	static const int LENGTH = 20; // Map을 그릴 때 블럭 한 간의 너비
	 
	// Map을 그릴 때 2차원 배열에 저장하여 판별
	static const int GROUND = 0;
	static const int WALL = 1;
	static const int FOOD = 2;
	static const int HEAD = 3;
	static const int BODY = 4;

private:
	int posX; // Map의 왼쪽 x좌표
	int posY; // Map의  위 y좌표
	int map[HEIGHT][WIDTH]; // Map 2차원 배열 생성

public:
	// private 변수 getter
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