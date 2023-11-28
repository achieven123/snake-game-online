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
	* @brief ���� ���� ���� �c��
	* @param count ������ ������ ��
	* @param board ���� ������ �ߺ� ���� �ʵ��� board ���� ��������
	* @details map�� ������ ��ġ�� ���� �����Ͽ� -> food �迭�� �Ҵ�
	*/
	void generateFood(int count, Board& board);

	/*
	* @brief ���� Ű �Է����� ���� ���� �Ǵ� �Լ�
	* @param x ���� ��ġ������ x ������
	* @param y ���� ��ġ������ y ������
	* @return ���� ��ġ�� location[1]�� ��ǥ�� ��ġ�Ѵٸ� True �ƴ϶�� False
	* @details
	* Snake�� ������ �������� �̵��ϱ� �� ������ ���� ����(dx == 1 �� �� VK_DOWN + VK_LEFT ��)��
	* �Է��Ͽ��� ��, �Ӹ��� ������ ��ġ�� ��ġ�Ͽ� ������ ����Ǵ� ���� �ذ�
	*/
	bool isOverlap(int x, int y);

	/*
	* @brief Snake�� ���� ���� ����
	* @param snake ������Ʈ�� Snake ��ü
	* @param directKey ����ڷ� ���� �Է¹��� ����Ű
	* @details ����ڰ� �Է��� ����� �ݴ� �������� �������� ���� �� ������ �ٲ�
	*/
	void setDirect(int directKey);
	
	/*
	* @brief Snake �̵� �Լ�
	* @param board ����, �浹 �Ǻ��� Board ��ü
	* @details dx, dy �������� Snake �̵�, �̵� �� �پ��� �̺�Ʈ ó��
	*/
	bool moveSnake(Board& board);

	/*
	* @brief Snake�� Value���� Map�� ����
	* @param board ����, �浹 �Ǻ��� Board ��ü
	* @details food, location ���� map�� ����
	*/
	void setMapValue(Board& board);
};