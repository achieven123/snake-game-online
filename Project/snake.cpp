#include "snake.h"

Snake::Snake() {
	dx = 1;
	dy = 0;
	score = 0;
	length = 3;

	location.push_back(make_pair(3, 1));
	location.push_back(make_pair(2, 1));
	location.push_back(make_pair(1, 1));
}

int Snake::getScore() { return score; }
int Snake::getLength() { return length; }
vector<pair<int, int>> Snake::getFood() { return food; }
vector<pair<int, int>> Snake::getLocation() { return location; }

void Snake::setScore(int score) { this->score = score; }
void Snake::setLength(int length) { this->length = length; }
void Snake::setFood(vector<pair<int, int>> food) { this->food = food; }
void Snake::setLocation(vector<pair<int, int>> location) { this->location = location; }

void Snake::generateFood(int count, Board& board) {
	int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();

	for (int i = 0; i < count; i++) {
		while (true) {
			int x = rand() % (Board::WIDTH - 2) + 1;
			int y = rand() % (Board::HEIGHT - 2) + 1;

			if (map[y][x] == Board::GROUND) {
				food.push_back(make_pair(x, y));
				break;
			}
		}
	}
}

bool Snake::isOverlap(int x, int y) {
	return (location[0].first + x == location[1].first) && (location[0].second + y == location[1].second);
}

void Snake::setDirect(int directKey) {
	switch (directKey) {
	case VK_LEFT:
		if (dx == 1 || isOverlap(-1, 0)) break;
		dx = -1; dy = 0;
		break;

	case VK_RIGHT:
		if (dx == -1 || isOverlap(1, 0)) break;
		dx = 1; dy = 0;
		break;

	case VK_UP:
		if (dy == 1 || isOverlap(0, -1)) break;
		dx = 0; dy = -1;
		break;

	case VK_DOWN:
		if (dy == -1 || isOverlap(0, 1)) break;
		dx = 0; dy = 1;
		break;
	}
}

bool Snake::moveSnake(Board& board) {
	int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();

	pair<int, int> head = location.front();
	pair<int, int> next = make_pair(head.first + dx, head.second + dy);
	pair<int, int> removeValue;

	switch (map[next.second][next.first]) {
	case Board::GROUND:
		location.pop_back();
		break;

	case Board::FOOD:
		removeValue = make_pair(next.first, next.second);
		food.erase(remove(food.begin(), food.end(), removeValue), food.end());
		generateFood(1, board);
		score++;
		if (length < 21) length++;
		break;

	case Board::WALL:
	case Board::BODY:
		return false;
	}

	location.insert(location.begin(), next);

	if (length == 21) {
		length--;
		location.pop_back();
	}

	return true;
}

void Snake::setMapValue(Board& board) {
	int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();
	int count = food.size();

	board.initMap(board.getPosX(), board.getPosY());

	for (int i = 0; i < count; i++) {
		pair<int, int> point = food[i];
		map[point.second][point.first] = Board::FOOD;
	}

	for (int i = 0; i < length; i++) {
		pair<int, int> point = location[i];
		map[point.second][point.first] = (i == 0) ? Board::HEAD : Board::BODY;
	}
}