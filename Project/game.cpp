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
    // Ŭ���̾�Ʈ ���� ���� �õ�
    bool openSuccessful = client.openClientSocket(hwnd, host, port);

    // ���ῡ ������ ���, ���� �޽��� ǥ�� �� false ��ȯ
    if (!openSuccessful) {
        MessageBox(hwnd, TEXT("���� ���� ����"), TEXT("Error"), MB_ICONERROR);
        return false;
    }

    // Ŭ���̾�Ʈ�� ���� ������ ���� (Ư�� �Ű����� ����)
    client.makeThread(snake2, msg, code, state);
    return true;
}

void Game::createRoom() {
    // �� ������ ���� JSON �޽��� ���� �� ������ ����
    string sendMsg = tools.createJsonData(snake1, "create");
    client.sendData(sendMsg);
}

bool Game::joinRoom(HWND hwnd, const string code) {
    // ������ �ڵ带 ����Ͽ� �� ������ ���� JSON �޽��� ���� �� ����
    string sendMsg = tools.createJsonData(snake1, "join " + code);
    client.sendData(sendMsg);

    // ���� ���� Ȯ���� ���� ª�� �ð� ���
    Sleep(50);

    // ������ ������ ��Ÿ���� ���, ���� �޽��� ǥ�� �� false ��ȯ
    if (msg == "join error") {
        MessageBox(hwnd, TEXT("���� �����ϴ�."), TEXT("Error"), MB_ICONERROR);
        return false;
    }

    return true;
}

void Game::startGame() {
    // ���� ������ ���� JSON �޽��� ���� �� ������ ����
    string msg = tools.createJsonData(snake1, "start");
    client.sendData(msg);
}

void Game::endGame() {
    // ���� ���Ḧ ���� JSON �޽��� ���� �� ������ ����
    string msg = tools.createJsonData(snake1, "end");
    client.sendData(msg);
}

void Game::initGame() {
    // ���� ���� �ʱ�ȭ, �� �ʱ�ȭ �� ���� ����
    board1.initMap(0, 0);
    board2.initMap(500, 0);

    srand((unsigned int)time(NULL));
    snake1.initSnake();
    snake1.generateFood(10, board1);
}

void Game::drawGame(HDC hdc) {
    // ���� ��忡 ���� ���� ��� �׸���
    wstring _code = tools.stringToWString("code: " + code);
    wstring _score1 = tools.stringToWString("score: " + to_string(snake1.getScore()));
    wstring _score2 = tools.stringToWString("score: " + to_string(snake2.getScore()));

    if (mode == SOLO) {
        // �ַ� ����� �ʰ� ���� �׸���
        snake1.setMapValue(board1);
        board1.drawMap(hdc);
        TextOut(hdc, 20, 450, _score1.c_str(), _score1.size());
    }
    else if (mode == MULTI) {
        // ��Ƽ�÷��̾� ����� ��, �ڵ�, ���� �׸���
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
    // ����� �Է¿� ���� ���� ���� ����
    snake1.setDirect(directKey);
}

void Game::moveSnake() {
    // ���� �����̰� �̵� �����͸� ������ ����
    bool move = snake1.moveSnake(board1);

    string msg = tools.createJsonData(snake1, "move");
    client.sendData(msg);

    // ���� ������ �� ���� ���, ���� ���� ǥ�� �� ������ �˸�
    if (!move) {
        isWin = false;
        msg = tools.createJsonData(snake1, "end");
        client.sendData(msg);
    }
}
