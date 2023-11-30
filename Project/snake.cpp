#include "snake.h"

// Snake 클래스의 생성자
Snake::Snake() {
	dx = 1; // 뱀의 초기 가로 이동 방향 설정 (오른쪽)
	dy = 0; // 뱀의 초기 세로 이동 방향 설정 (정지)
	score = 0; // 뱀의 초기 점수 설정
	length = 3; // 뱀의 초기 길이 설정

	// 뱀의 초기 위치 설정
	location.push_back(make_pair(3, 1)); // 머리
	location.push_back(make_pair(2, 1)); // 몸통
	location.push_back(make_pair(1, 1)); // 꼬리
}

// 뱀 초기화 함수
void Snake::initSnake() {
	dx = 1; // 뱀의 초기 가로 이동 방향 설정 (오른쪽)
	dy = 0; // 뱀의 초기 세로 이동 방향 설정 (정지)
	score = 0; // 뱀의 초기 점수 설정
	length = 3; // 뱀의 초기 길이 설정

	location.clear(); // 뱀의 위치 정보 초기화

	// 다시 뱀의 초기 위치 설정
	location.push_back(make_pair(3, 1)); // 머리
	location.push_back(make_pair(2, 1)); // 몸통
	location.push_back(make_pair(1, 1)); // 꼬리

	food.clear(); // 먹이 정보 초기화
}

int Snake::getScore() { return score; }
int Snake::getLength() { return length; }
vector<pair<int, int>> Snake::getFood() { return food; }
vector<pair<int, int>> Snake::getLocation() { return location; }

void Snake::setScore(int score) { this->score = score; }
void Snake::setLength(int length) { this->length = length; }
void Snake::setFood(vector<pair<int, int>> food) { this->food = food; }
void Snake::setLocation(vector<pair<int, int>> location) { this->location = location; }

// 게임 보드에 지정된 개수의 먹이를 생성하는 함수
void Snake::generateFood(int count, Board& board) {
	// 보드의 맵 정보를 참조하는 배열 맵 선언
	int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();

	// 지정된 개수만큼 먹이를 생성
	for (int i = 0; i < count; i++) {
		// 무한 루프: 유효한 위치에 먹이 생성할 때까지 반복
		while (true) {
			// 1부터 Board의 너비와 높이에서 2를 뺀 범위 내에서 무작위 좌표 생성
			int x = rand() % (Board::WIDTH - 2) + 1;
			int y = rand() % (Board::HEIGHT - 2) + 1;

			// 선택된 위치가 'GROUND'인지 확인하여 유효한 위치인지 확인
			if (map[y][x] == Board::GROUND) {
				// 선택된 위치가 'GROUND'일 경우 해당 위치에 먹이 추가
				food.push_back(make_pair(x, y));
				break; // 유효한 위치에서 먹이 추가 후 무한 루프 종료
			}
		}
	}
}

// 뱀의 머리와 몸통이 겹치는지 확인하는 함수
bool Snake::isOverlap(int x, int y) {
    // 머리와 몸통 좌표가 겹치는지 확인하여 겹치면 true 반환
    return (location[0].first + x == location[1].first) && (location[0].second + y == location[1].second);
}

// 뱀의 이동 방향 설정 함수
void Snake::setDirect(int directKey) {
    switch (directKey) {
    case VK_LEFT:
        // 오른쪽으로 이동 중이거나 왼쪽으로 이동시 머리와 몸통이 겹치면 방향 변경하지 않음
        if (dx == 1 || isOverlap(-1, 0)) break;
        dx = -1; dy = 0; // 왼쪽으로 이동 설정
        break;

    case VK_RIGHT:
        // 왼쪽으로 이동 중이거나 오른쪽으로 이동시 머리와 몸통이 겹치면 방향 변경하지 않음
        if (dx == -1 || isOverlap(1, 0)) break;
        dx = 1; dy = 0; // 오른쪽으로 이동 설정
        break;

    case VK_UP:
        // 아래쪽으로 이동 중이거나 위쪽으로 이동시 머리와 몸통이 겹치면 방향 변경하지 않음
        if (dy == 1 || isOverlap(0, -1)) break;
        dx = 0; dy = -1; // 위쪽으로 이동 설정
        break;

    case VK_DOWN:
        // 위쪽으로 이동 중이거나 아래쪽으로 이동시 머리와 몸통이 겹치면 방향 변경하지 않음
        if (dy == -1 || isOverlap(0, 1)) break;
        dx = 0; dy = 1; // 아래쪽으로 이동 설정
        break;
    }
}

// 뱀의 이동 처리 함수
bool Snake::moveSnake(Board& board) {
    int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();

    pair<int, int> head = location.front(); // 뱀의 머리 위치
    pair<int, int> next = make_pair(head.first + dx, head.second + dy); // 다음 이동할 위치

    pair<int, int> removeValue;

    switch (map[next.second][next.first]) {
    case Board::GROUND:
        location.pop_back(); // 뱀의 꼬리 위치 이동
        break;

    case Board::FOOD:
        removeValue = make_pair(next.first, next.second);
        food.erase(remove(food.begin(), food.end(), removeValue), food.end()); // 먹이 삭제
        generateFood(1, board); // 새로운 먹이 생성
        score += 100; // 점수 증가
        if (length < 21) length++; // 최대 길이 21보다 작으면 길이 증가
        break;

    case Board::WALL:
    case Board::BODY:
        return false; // 벽이나 몸통과 충돌시 게임 종료를 위해 false 반환
    }

    location.insert(location.begin(), next); // 머리를 새로운 위치로 이동

    // 뱀의 길이가 최대 길이인 21이면 꼬리 위치 이동
    if (length == 21) {
        length--;
        location.pop_back();
    }

    return true; // 정상적인 이동이 완료되면 true 반환
}

// 게임 보드에 뱀과 먹이를 나타내는 값 설정 함수
void Snake::setMapValue(Board& board) {
    int(&map)[Board::HEIGHT][Board::WIDTH] = board.getMap();
    int count = food.size();

    board.initMap(board.getPosX(), board.getPosY()); // 보드 초기화

    // 먹이 위치 설정
    for (int i = 0; i < count; i++) {
        pair<int, int> point = food[i];
        map[point.second][point.first] = Board::FOOD;
    }

    // 뱀 위치 설정
    for (int i = 0; i < length; i++) {
        pair<int, int> point = location[i];
        map[point.second][point.first] = (i == 0) ? Board::HEAD : Board::BODY;
    }
}
