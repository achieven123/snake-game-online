#include "game.h"

bool Game::initGame(HWND hwnd, const char* host, const char* port) {
	bool openSuccessful = client.openClientSocket(hwnd, host, port);
	
	if (!openSuccessful) {
		MessageBox(hwnd, TEXT("서버 연결 실패"), TEXT("Error"), MB_ICONERROR);
		return false;
	}

	client.makeThread(snake1, snake2);
	board1.initMap(0, 0);
	board2.initMap(500, 0);
	snake1.generateFood(10, board1.getMap());

	return true;
}

void Game::createRoom() {
	Tools tools;
	string msg = tools.CreateJsonData(snake1, "create");
	client.SendData(msg);
}

bool Game::joinRoom(HWND hwnd, string code) {
	Tools tools;
	
	snake1.setCode(code);
	
	string msg = tools.CreateJsonData(snake1, "join");
	client.SendData(msg);

	Sleep(100);
	string recvMsg = snake1.getMsg();

	if (recvMsg == "Join error!") {
		MessageBox(hwnd, TEXT("방이 없습니다."), TEXT("Error"), MB_ICONERROR);
		return false;
	}

	snake1.setCode(code);

	return true;
}

void Game::endGame() {
	client.closeClientSocket();
}

void Game::setDirect(int directKey) {
	snake1.setDirect(snake1, directKey);
}

bool Game::moveSnake(HDC hdc) {
	bool gameover = snake1.moveSnake(snake1, board1);

	snake1.setMapValue(snake1, board1);
	snake2.setMapValue(snake2, board2);

	board1.drawMap(hdc);
	board2.drawMap(hdc);

	Tools tools;
	string msg = tools.CreateJsonData(snake1, "move");
	client.SendData(msg);
	
	return gameover;
}

string Game::getSnakeCode() {
	return snake1.getCode();
}