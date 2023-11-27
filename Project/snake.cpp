#include "snake.h"

Snake::Snake() {
	code = "";
	score = 0;
	length = 3;
	dx = 1;
	dy = 0;

	location.push_back(make_pair(3, 1));
	location.push_back(make_pair(2, 1));
	location.push_back(make_pair(1, 1));
}

string Snake::getMsg() { return msg; }
string Snake::getCode() { return code; }
int Snake::getScore() { return score; }
int Snake::getLength() { return length; }
int Snake::getDx() { return dx; }
int Snake::getDy() { return dy; }
vector<pair<int, int>> Snake::getFood() { return food; }
vector<pair<int, int>> Snake::getLocation() { return location; }

void Snake::setMsg(string msg) { this->msg = msg; }
void Snake::setCode(string code) { this->code = code; }
void Snake::setScore(int score) { this->score = score; }
void Snake::setDx(int dx) { this->dx = dx; }
void Snake::setDy(int dy) { this->dy = dy; }
void Snake::setLength(int length) { this->length = length; }
void Snake::setFood(vector<pair<int, int>> food) { this->food = food; }
void Snake::setLocation(vector<pair<int, int>> location) { this->location = location; }

void Snake::printSnake() {
	cout << "[Snake Info]" << endl;
	cout << "code: " << code << endl;
	cout << "xDirect: " << dx << endl;
	cout << "yDirect: " << dy << endl;
	cout << "length: " << length << endl;
	cout << "loaction: " << endl;

	for (const auto& point : getLocation()) {
		cout << "(" << point.first << ", " << point.second << ")" << endl;
	}
	cout << "food: " << endl;

	for (const auto& point : food) {
		cout << "(" << point.first << ", " << point.second << ")" << endl;
	}
}

/*
* @brief ���� Ű �Է����� ���� ���� �Ǵ� �Լ�
* @param x ���� ��ġ������ x ������
* @param y ���� ��ġ������ y ������
* @return ���� ��ġ�� location[1]�� ��ǥ�� ��ġ�Ѵٸ� True �ƴ϶�� False
* @details
* Snake�� ������ �������� �̵��ϱ� �� ������ ���� ����(xDirect == 1 �� �� VK_DOWN + VK_LEFT ��)��
* �Է��Ͽ��� ��, �Ӹ��� ������ ��ġ�� ��ġ�Ͽ� ������ ����Ǵ� ���� �ذ�
*/
bool Snake::isOverlap(int x, int y) {
	return (location[0].first + x == location[1].first) && (location[0].second + y == location[1].second);
}

/*
* @brief Snake�� ���� ���� ����
* @param snake ������Ʈ�� Snake ��ü�� ���� ���۷���
* @param directKey ����ڷ� ���� �Է¹��� ����Ű
* @details ����ڰ� �Է��� ����� �ݴ� �������� �������� ���� �� ������ �ٲ�
*/
void Snake::setDirect(Snake& snake, int directKey) {
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

void Snake::generateFood(int count, int(&map)[Board::HEIGHT][Board::WIDTH]) {
	for (int i = 0; i < count; i++) {
		while (true) {
			int x = rand() % (Board::WIDTH - 2) + 1;
			int y = rand() % (Board::HEIGHT - 2) + 1;

			if (map[y][x] == Board::GROUND) {
				food.push_back(make_pair(x, y));
				//map[y][x] = Board::FOOD;
				break;
			}
		}
	}
}

bool Snake::moveSnake(Snake& snake, Board& board) {
	int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();
	pair<int, int> head, next, point;

	pair<int, int> valueToRemove;

	head = location.front();
	next = make_pair(head.first + dx, head.second + dy);

	switch (map[next.second][next.first]) {
	case Board::GROUND:
		point = location.back();
		//map[point.second][point.first] = Board::GROUND;
		location.pop_back();
		break;

	case Board::FOOD:
		score++;
		valueToRemove = make_pair(next.first, next.second);
		food.erase(std::remove(food.begin(), food.end(), valueToRemove), food.end());
		//map[next.second][next.first] = Board::GROUND;
		generateFood(1, map);
		if (length < 21) length++;
		break;

	case Board::WALL:
	case Board::BODY:
		return true;
	}

	location.insert(location.begin(), next);

	if (length == 21) {
		length--;
		location.pop_back();
	}
	return false;
}

void Snake::setMapValue(Snake& snake, Board& board) {
	int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();
	pair<int, int> point;

	int count = food.size();

	board.initMap(board.getPosX(), board.getPosY());

	for (int i = 0; i < count; i++) {
		point = food[i];
		map[point.second][point.first] = Board::FOOD;
	}

	for (int i = 0; i < length; i++) {
		point = location[i];
		map[point.second][point.first] = (i == 0) ? Board::HEAD : Board::BODY;
	}
}