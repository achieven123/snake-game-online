#pragma once

#include <Windows.h>

class Board {
public:
	static const int HEIGHT = 22; // Map�� ����
	static const int WIDTH = 22; // Map�� �ʺ�
	static const int LENGTH = 20; // Map�� �׸� �� �� �� ���� �ʺ�
	 
	// Map�� �׸� �� 2���� �迭�� �����Ͽ� �Ǻ�
	static const int GROUND = 0;
	static const int WALL = 1;
	static const int FOOD = 2;
	static const int HEAD = 3;
	static const int BODY = 4;

private:
	int posX; // Map�� ���� x��ǥ
	int posY; // Map��  �� y��ǥ
	int map[HEIGHT][WIDTH]; // Map 2���� �迭 ����

public:
	// private ���� getter
	int getPosX();
	int getPosY();
	int(&getMap())[HEIGHT][WIDTH];
	
	/*
	* @brief map �ʱ�ȭ �Լ�
	* @param posX, posY map ���� ��ġ
	* @details map �ʱ� ��ġ�� ���ϰ�, �ʱ�ȭ
	*/
	void initMap(int posX, int posY);
	
	/*
	* @brief map �׸��� �Լ�
	* @param hdc ����̽� ���ؽ�Ʈ�� ���� �ڵ�
	* @details 2���� �迭 �ȿ� �ִ� ���� ���� �׸� �׸���
	*/
	void drawMap(HDC hdc);
};