#include "snake.h"

// Snake Ŭ������ ������
Snake::Snake() {
	dx = 1; // ���� �ʱ� ���� �̵� ���� ���� (������)
	dy = 0; // ���� �ʱ� ���� �̵� ���� ���� (����)
	score = 0; // ���� �ʱ� ���� ����
	length = 3; // ���� �ʱ� ���� ����

	// ���� �ʱ� ��ġ ����
	location.push_back(make_pair(3, 1)); // �Ӹ�
	location.push_back(make_pair(2, 1)); // ����
	location.push_back(make_pair(1, 1)); // ����
}

// �� �ʱ�ȭ �Լ�
void Snake::initSnake() {
	dx = 1; // ���� �ʱ� ���� �̵� ���� ���� (������)
	dy = 0; // ���� �ʱ� ���� �̵� ���� ���� (����)
	score = 0; // ���� �ʱ� ���� ����
	length = 3; // ���� �ʱ� ���� ����

	location.clear(); // ���� ��ġ ���� �ʱ�ȭ

	// �ٽ� ���� �ʱ� ��ġ ����
	location.push_back(make_pair(3, 1)); // �Ӹ�
	location.push_back(make_pair(2, 1)); // ����
	location.push_back(make_pair(1, 1)); // ����

	food.clear(); // ���� ���� �ʱ�ȭ
}

int Snake::getScore() { return score; }
int Snake::getLength() { return length; }
vector<pair<int, int>> Snake::getFood() { return food; }
vector<pair<int, int>> Snake::getLocation() { return location; }

void Snake::setScore(int score) { this->score = score; }
void Snake::setLength(int length) { this->length = length; }
void Snake::setFood(vector<pair<int, int>> food) { this->food = food; }
void Snake::setLocation(vector<pair<int, int>> location) { this->location = location; }

// ���� ���忡 ������ ������ ���̸� �����ϴ� �Լ�
void Snake::generateFood(int count, Board& board) {
	// ������ �� ������ �����ϴ� �迭 �� ����
	int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();

	// ������ ������ŭ ���̸� ����
	for (int i = 0; i < count; i++) {
		// ���� ����: ��ȿ�� ��ġ�� ���� ������ ������ �ݺ�
		while (true) {
			// 1���� Board�� �ʺ�� ���̿��� 2�� �� ���� ������ ������ ��ǥ ����
			int x = rand() % (Board::WIDTH - 2) + 1;
			int y = rand() % (Board::HEIGHT - 2) + 1;

			// ���õ� ��ġ�� 'GROUND'���� Ȯ���Ͽ� ��ȿ�� ��ġ���� Ȯ��
			if (map[y][x] == Board::GROUND) {
				// ���õ� ��ġ�� 'GROUND'�� ��� �ش� ��ġ�� ���� �߰�
				food.push_back(make_pair(x, y));
				break; // ��ȿ�� ��ġ���� ���� �߰� �� ���� ���� ����
			}
		}
	}
}

// ���� �Ӹ��� ������ ��ġ���� Ȯ���ϴ� �Լ�
bool Snake::isOverlap(int x, int y) {
    // �Ӹ��� ���� ��ǥ�� ��ġ���� Ȯ���Ͽ� ��ġ�� true ��ȯ
    return (location[0].first + x == location[1].first) && (location[0].second + y == location[1].second);
}

// ���� �̵� ���� ���� �Լ�
void Snake::setDirect(int directKey) {
    switch (directKey) {
    case VK_LEFT:
        // ���������� �̵� ���̰ų� �������� �̵��� �Ӹ��� ������ ��ġ�� ���� �������� ����
        if (dx == 1 || isOverlap(-1, 0)) break;
        dx = -1; dy = 0; // �������� �̵� ����
        break;

    case VK_RIGHT:
        // �������� �̵� ���̰ų� ���������� �̵��� �Ӹ��� ������ ��ġ�� ���� �������� ����
        if (dx == -1 || isOverlap(1, 0)) break;
        dx = 1; dy = 0; // ���������� �̵� ����
        break;

    case VK_UP:
        // �Ʒ������� �̵� ���̰ų� �������� �̵��� �Ӹ��� ������ ��ġ�� ���� �������� ����
        if (dy == 1 || isOverlap(0, -1)) break;
        dx = 0; dy = -1; // �������� �̵� ����
        break;

    case VK_DOWN:
        // �������� �̵� ���̰ų� �Ʒ������� �̵��� �Ӹ��� ������ ��ġ�� ���� �������� ����
        if (dy == -1 || isOverlap(0, 1)) break;
        dx = 0; dy = 1; // �Ʒ������� �̵� ����
        break;
    }
}

// ���� �̵� ó�� �Լ�
bool Snake::moveSnake(Board& board) {
    int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();

    pair<int, int> head = location.front(); // ���� �Ӹ� ��ġ
    pair<int, int> next = make_pair(head.first + dx, head.second + dy); // ���� �̵��� ��ġ

    pair<int, int> removeValue;

    switch (map[next.second][next.first]) {
    case Board::GROUND:
        location.pop_back(); // ���� ���� ��ġ �̵�
        break;

    case Board::FOOD:
        removeValue = make_pair(next.first, next.second);
        food.erase(remove(food.begin(), food.end(), removeValue), food.end()); // ���� ����
        generateFood(1, board); // ���ο� ���� ����
        score += 100; // ���� ����
        if (length < 21) length++; // �ִ� ���� 21���� ������ ���� ����
        break;

    case Board::WALL:
    case Board::BODY:
        return false; // ���̳� ����� �浹�� ���� ���Ḧ ���� false ��ȯ
    }

    location.insert(location.begin(), next); // �Ӹ��� ���ο� ��ġ�� �̵�

    // ���� ���̰� �ִ� ������ 21�̸� ���� ��ġ �̵�
    if (length == 21) {
        length--;
        location.pop_back();
    }

    return true; // �������� �̵��� �Ϸ�Ǹ� true ��ȯ
}

// ���� ���忡 ��� ���̸� ��Ÿ���� �� ���� �Լ�
void Snake::setMapValue(Board& board) {
    int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();
    int count = food.size();

    board.initMap(board.getPosX(), board.getPosY()); // ���� �ʱ�ȭ

    // ���� ��ġ ����
    for (int i = 0; i < count; i++) {
        pair<int, int> point = food[i];
        map[point.second][point.first] = Board::FOOD;
    }

    // �� ��ġ ����
    for (int i = 0; i < length; i++) {
        pair<int, int> point = location[i];
        map[point.second][point.first] = (i == 0) ? Board::HEAD : Board::BODY;
    }
}
