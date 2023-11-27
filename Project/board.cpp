#include "board.h"

int Board::getPosX() { return posX; }
int Board::getPosY() { return posY; }


int(&Board::getMap())[HEIGHT][WIDTH] { return map; }

void Board::initMap(int posX, int posY) {
	this->posX = posX;
	this->posY = posY;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			map[y][x] = 0;
		}
	}

	for (int y = 0; y < HEIGHT; y++) {
		map[y][0] = map[y][WIDTH - 1] = WALL;
	}

	for (int x = 0; x < WIDTH; x++) {
		map[0][x] = map[HEIGHT - 1][x] = WALL;
	}
}

void Board::drawMap(HDC hdc) {
	HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN bluePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			switch (map[y][x]) {
			case FOOD:
				SelectObject(hdc, GetStockObject(BLACK_BRUSH));
				break;

			case HEAD:
				SelectObject(hdc, redPen);
				break;
			
			case BODY:
				SelectObject(hdc, bluePen);
				break;
			}

			switch (map[y][x]) {
			case WALL:
				Rectangle(
					hdc,
					x * LENGTH + posX,
					y * LENGTH + posY,
					(x + 1) * LENGTH + posX,
					(y + 1) * LENGTH + posY
				);

				break;

			case FOOD:
			case HEAD:
			case BODY:
				Ellipse(
					hdc,
					x * LENGTH + posX,
					y * LENGTH + posY,
					(x + 1) * LENGTH + posX,
					(y + 1) * LENGTH + posY
				);

				break;
			}

			SelectObject(hdc, GetStockObject(WHITE_BRUSH));
			SelectObject(hdc, GetStockObject(BLACK_PEN));
		}
	}
}