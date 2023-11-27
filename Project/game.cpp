#include "game.h"

void Game::initGame(HWND hwnd) {
	client.openClientSocket(hwnd, HOST, PORT);
	client.makeThread(snake1, snake2);

	board1.initMap(0, 0);
	board2.initMap(500, 0);
}

void Game::startGame() {
	Tools tools;
	
	snake1.generateFood(10, board1.getMap());
	string msg = tools.CreateJsonData(snake1, "create");
	client.SendData(msg);

	msg = tools.CreateJsonData(snake2, "join " + snake1.getCode());
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

