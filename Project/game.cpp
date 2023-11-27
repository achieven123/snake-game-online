#include "game.h"

bool Game::initGame(HWND hwnd, const char* host, const char* port) {
	bool openSuccessful = client.openClientSocket(hwnd, host, port);
	
	if (!openSuccessful) {
		MessageBox(hwnd, TEXT("서버 연결 실패"), TEXT("Error"), MB_ICONERROR);
		return false;
	}

	client.makeThread(snake1, snake2);


	return true;
}

void Game::createRoom() {
	Tools tools;
	string msg = tools.CreateJsonData(snake1, "create");
	client.SendData(msg);
	snake1.printSnake();
	snake2.printSnake();

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
	snake1.printSnake();
	snake2.printSnake();
	return true;
}

void Game::endGame() {
	client.closeClientSocket();
}

void Game::setDirect(int directKey) {
	snake1.setDirect(snake1, directKey);
}

void Game::startGame(bool multi) {
	srand((unsigned int)time(NULL));

	snake1.generateFood(10, board1.getMap());
	board1.initMap(0, 0);
	board2.initMap(500, 0);
}

bool Game::moveSnake(HDC hdc, bool multi) {
	Tools tools;
	bool gameover = snake1.moveSnake(snake1, board1);
		
	snake1.setMapValue(snake1, board1);
	board1.drawMap(hdc);

	string _code = "code: " + snake1.getCode();
	wstring code(_code.begin(), _code.end());

	string _score1 = "score: " + to_string(snake1.getScore());
	wstring score1(_score1.begin(), _score1.end());
	
	string _score2 = "score: " + to_string(snake2.getScore());
	wstring score2(_score2.begin(), _score2.end());
	
	if (multi) {
		snake2.setMapValue(snake2, board2);
		board2.drawMap(hdc);

		string msg = tools.CreateJsonData(snake1, "move");
		client.SendData(msg);

		TextOut(hdc, 20, 450, code.c_str(), code.length());
		TextOut(hdc, 120, 450, score1.c_str(), score1.length());
		TextOut(hdc, 520, 450, score2.c_str(), score2.length());
	}
	else {
		TextOut(hdc, 20, 450, score1.c_str(), score1.length());
	}

	
	return gameover;
}

string Game::getSnakeCode() {
	return snake1.getCode();
}

bool Game::IsTwoPlayer() {
	return snake2.getCode() != "";
}