#include "game.h"

string Game::getCode() { return code; }
int Game::getMode() { return mode; }
int Game::getType() { return type; }
int Game::getState() { return state; }
bool Game::getIsWin() { return isWin; }

void Game::setMode(int mode) { this->mode = mode; }
void Game::setType(int type) { this->type = type; }
void Game::setState(int state) { this->state = state; }
void Game::setIsWin(bool isWin) { this->isWin = isWin; }

bool Game::connectServer(HWND hwnd, const char* host, const char* port) {
	bool openSuccessful = client.openClientSocket(hwnd, host, port);
	
	if (!openSuccessful) {
		MessageBox(hwnd, TEXT("서버 연결 실패"), TEXT("Error"), MB_ICONERROR);
		return false;
	}

	client.makeThread(snake2, msg, code, state);
	return true;
}

void Game::createRoom() {
	string sendMsg = tools.createJsonData(snake1, "create");
	client.sendData(sendMsg);
}

bool Game::joinRoom(HWND hwnd, const string code) {
	string sendMsg = tools.createJsonData(snake1, "join " + code);
	client.sendData(sendMsg);

	Sleep(50);
	if (msg == "join error") {
		MessageBox(hwnd, TEXT("방이 없습니다."), TEXT("Error"), MB_ICONERROR);
		return false;
	}

	return true;
}

void Game::startGame() {
	string msg = tools.createJsonData(snake1, "start");
	client.sendData(msg);
}

void Game::endGame() {
	string msg = tools.createJsonData(snake1, "end");
	client.sendData(msg);
}

void Game::initGame() {
	board1.initMap(0, 0);
	board2.initMap(500, 0);

	srand((unsigned int)time(NULL));
	snake1.initSnake();
	snake1.generateFood(10, board1);
}

void Game::drawGame(HDC hdc) {
	wstring _code = tools.stringToWString("code: " + code);
	wstring _score1 = tools.stringToWString("score: " + to_string(snake1.getScore()));
	wstring _score2 = tools.stringToWString("score: " + to_string(snake2.getScore()));

	if (mode == SOLO) {
		snake1.setMapValue(board1);
		board1.drawMap(hdc);

		TextOut(hdc, 20, 450, _score1.c_str(), _score1.size());
	}
	else if (mode == MULTI) {
		snake1.setMapValue(board1);
		board1.drawMap(hdc);

		snake2.setMapValue(board2);
		board2.drawMap(hdc);

		TextOut(hdc, 20, 450, _code.c_str(), _code.size());
		TextOut(hdc, 120, 450, _score1.c_str(), _score1.size());
		TextOut(hdc, 520, 450, _score2.c_str(), _score2.size());
	}
}

void Game::setDirect(int directKey) {
	snake1.setDirect(directKey);
}

void Game::moveSnake() {
	bool move = snake1.moveSnake(board1);

	string msg = tools.createJsonData(snake1, "move");
	client.sendData(msg);

	if (!move) {
		isWin = false;
		msg = tools.createJsonData(snake1, "end");
		client.sendData(msg);
	}
}
