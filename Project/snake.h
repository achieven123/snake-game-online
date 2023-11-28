#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
#include "board.h"

using namespace std;

class Snake {
private:
	int dx;
	int dy;
	int score;
	int length;
	vector<pair<int, int>> food;
	vector<pair<int, int>> location;

public:
	Snake();
	
	int getScore();
	int getLength();
	vector<pair<int, int>> getFood();
	vector<pair<int, int>> getLocation();

	void setScore(int score);
	void setLength(int length);
	void setFood(vector<pair<int, int>> food);
	void setLocation(vector<pair<int, int>> location);

	/*
	* @brief 음식 랜덤 생성 홤수
	* @param count 생성할 음식의 수
	* @param board 음식 생성이 중복 되지 않도록 board 정보 가져오기
	* @details map의 랜덤한 위치에 음식 생성하여 -> food 배열에 할당
	*/
	void generateFood(int count, Board& board);

	/*
	* @brief 빠른 키 입력으로 인한 오류 판단 함수
	* @param x 다음 위치에서의 x 증가량
	* @param y 다음 위치에서의 y 증가량
	* @return 다음 위치가 location[1]의 좌표와 일치한다면 True 아니라면 False
	* @details
	* Snake가 지정한 방향으로 이동하기 전 다음과 같은 조합(dx == 1 일 때 VK_DOWN + VK_LEFT 등)을
	* 입력하였을 때, 머리와 몸통의 위치가 일치하여 게임이 종료되는 오류 해결
	*/
	bool isOverlap(int x, int y);

	/*
	* @brief Snake의 진행 방향 설정
	* @param snake 업데이트할 Snake 객체
	* @param directKey 사용자로 부터 입력받은 방향키
	* @details 사용자가 입력한 방향과 반대 방향으로 움직이지 않을 때 방향을 바꿈
	*/
	void setDirect(int directKey);
	
	/*
	* @brief Snake 이동 함수
	* @param board 음식, 충돌 판별할 Board 객체
	* @details dx, dy 방향으로 Snake 이동, 이동 중 다양한 이벤트 처리
	*/
	bool moveSnake(Board& board);

	/*
	* @brief Snake의 Value들을 Map에 적용
	* @param board 음식, 충돌 판별할 Board 객체
	* @details food, location 값을 map에 적용
	*/
	void setMapValue(Board& board);
};