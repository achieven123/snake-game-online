#include "game.h"

string Game::getCode() { return code; }
int Game::getPlayer() { return player; }
int Game::getState() { return state; }

void Game::setState(int state) { this->state = state; }

bool Game::connectServer(HWND hwnd, const char* host, const char* port) {
	bool openSuccessful = client.openClientSocket(hwnd, host, port);
	
	if (!openSuccessful) {
		MessageBox(hwnd, TEXT("서버 연결 실패"), TEXT("Error"), MB_ICONERROR);
		return false;
	}

	client.makeThread(snake2, msg, code, player, state);
	return true;
}

void Game::initGame() {
	srand((unsigned int)time(NULL));
	snake1.generateFood(10, board1);
	board1.initMap(0, 0);
	board2.initMap(500, 0);
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

void Game::setDirect(int directKey) {
	snake1.setDirect(directKey);
}

void Game::drawGame(HDC hdc) {
	wstring _code = tools.stringToWString("code: " + code);
	wstring _score1 = tools.stringToWString("score: " + to_string(snake1.getScore()));
	wstring _score2 = tools.stringToWString("score: " + to_string(snake2.getScore()));

	if (player < 2) {
		snake1.setMapValue(board1);
		board1.drawMap(hdc);

		TextOut(hdc, 120, 450, _score1.c_str(), _score1.size());
	}
	else {
		snake2.setMapValue(board2);
		board2.drawMap(hdc);

		string msg = tools.createJsonData(snake1, "move");
		client.sendData(msg);

		TextOut(hdc, 20, 450, _code.c_str(), _code.size());
		TextOut(hdc, 120, 450, _score1.c_str(), _score1.size());
		TextOut(hdc, 520, 450, _score2.c_str(), _score2.size());
	}
}

bool Game::moveSnake(HDC hdc, bool multi) {
	bool gameover = false;
	if (true) {
		//bool gameover = snake1.moveSnake(snake1, board1);
		//drawGame(hdc, multi);
	}

	return gameover;
}