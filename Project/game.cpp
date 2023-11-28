#include "game.h"

bool Game::connectServer(HWND hwnd, const char* host, const char* port) {
	bool openSuccessful = client.openClientSocket(hwnd, host, port);
	
	if (!openSuccessful) {
		MessageBox(hwnd, TEXT("서버 연결 실패"), TEXT("Error"), MB_ICONERROR);
		return false;
	}

	client.makeThread(snake2, msg, code, player);
	return true;
}

void Game::initGame() {
	srand((unsigned int)time(NULL));
	snake1.generateFood(10, board1);
	board1.initMap(0, 0);
	board2.initMap(500, 0);
}

void Game::createRoom() {
	string msg = tools.createJsonData(snake1, "create");
	client.sendData(msg);
}

bool Game::joinRoom(HWND hwnd, string code) {
	
	string msg = tools.createJsonData(snake1, "join");
	client.sendData(msg);

	Sleep(100);
	//if (recvMsg == "Join error!") {
	//	MessageBox(hwnd, TEXT("방이 없습니다."), TEXT("Error"), MB_ICONERROR);
	//	return false;
	//}

	return true;
}



void Game::endGame() {
	client.closeClientSocket();
}

void Game::setDirect(int directKey) {
	snake1.setDirect(snake1, directKey);
}

void Game::startGame() {
	gameProgress = true;
	string msg = tools.createJsonData(snake1, "start");
}

void Game::drawGame(HDC hdc, bool multi) {
	string _code = "code: " /*+ snake1.getCode()*/;
	wstring code(_code.begin(), _code.end());

	string _score1 = "score: " + to_string(snake1.getScore());
	wstring score1(_score1.begin(), _score1.end());

	string _score2 = "score: " + to_string(snake2.getScore());
	wstring score2(_score2.begin(), _score2.end());

	snake1.setMapValue(snake1, board1);
	board1.drawMap(hdc);

	if (multi) {
		snake2.setMapValue(snake2, board2);
		board2.drawMap(hdc);

		string msg = tools.createJsonData(snake1, "move");
		client.sendData(msg);

		TextOut(hdc, 20, 450, code.c_str(), code.length());
		TextOut(hdc, 120, 450, score1.c_str(), score1.length());
		TextOut(hdc, 520, 450, score2.c_str(), score2.length());
	}
	else {
		TextOut(hdc, 20, 450, score1.c_str(), score1.length());
	}
}

bool Game::moveSnake(HDC hdc, bool multi) {
	bool gameover = false;
	if (gameProgress) {
		bool gameover = snake1.moveSnake(snake1, board1);
		drawGame(hdc, multi);
	}

	return gameover;
}
//
//string Game::getSnakeCode() {
//	return snake1.getCode();
//}
//
//bool Game::IsTwoPlayer() {
//	return snake2.getCode() != "";
//}
//
//int Game::getPlayer() {
//	return snake1.getPlayer();
//}