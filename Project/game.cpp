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
    // 클라이언트 소켓 열기 시도
    bool openSuccessful = client.openClientSocket(hwnd, host, port);

    // 연결에 실패한 경우, 오류 메시지 표시 후 false 반환
    if (!openSuccessful) {
        MessageBox(hwnd, TEXT("서버 연결 실패"), TEXT("Error"), MB_ICONERROR);
        return false;
    }

    // 클라이언트를 위한 스레드 생성 (특정 매개변수 포함)
    client.makeThread(snake2, msg, code, state);
    return true;
}

void Game::createRoom() {
    // 방 생성을 위한 JSON 메시지 생성 후 서버에 전송
    string sendMsg = tools.createJsonData(snake1, "create");
    client.sendData(sendMsg);
}

bool Game::joinRoom(HWND hwnd, const string code) {
    // 제공된 코드를 사용하여 방 참가를 위한 JSON 메시지 생성 후 전송
    string sendMsg = tools.createJsonData(snake1, "join " + code);
    client.sendData(sendMsg);

    // 서버 응답 확인을 위해 짧은 시간 대기
    Sleep(50);

    // 응답이 오류를 나타내는 경우, 오류 메시지 표시 후 false 반환
    if (msg == "join error") {
        MessageBox(hwnd, TEXT("방이 없습니다."), TEXT("Error"), MB_ICONERROR);
        return false;
    }

    return true;
}

void Game::startGame() {
    // 게임 시작을 위한 JSON 메시지 생성 후 서버에 전송
    string msg = tools.createJsonData(snake1, "start");
    client.sendData(msg);
}

void Game::endGame() {
    // 게임 종료를 위한 JSON 메시지 생성 후 서버에 전송
    string msg = tools.createJsonData(snake1, "end");
    client.sendData(msg);
}

void Game::initGame() {
    // 게임 보드 초기화, 뱀 초기화 및 먹이 생성
    board1.initMap(0, 0);
    board2.initMap(500, 0);

    srand((unsigned int)time(NULL));
    snake1.initSnake();
    snake1.generateFood(10, board1);
}

void Game::drawGame(HDC hdc) {
    // 게임 모드에 따라 게임 요소 그리기
    wstring _code = tools.stringToWString("code: " + code);
    wstring _score1 = tools.stringToWString("score: " + to_string(snake1.getScore()));
    wstring _score2 = tools.stringToWString("score: " + to_string(snake2.getScore()));

    if (mode == SOLO) {
        // 솔로 모드의 맵과 점수 그리기
        snake1.setMapValue(board1);
        board1.drawMap(hdc);
        TextOut(hdc, 20, 450, _score1.c_str(), _score1.size());
    }
    else if (mode == MULTI) {
        // 멀티플레이어 모드의 맵, 코드, 점수 그리기
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
    // 사용자 입력에 따라 뱀의 방향 설정
    snake1.setDirect(directKey);
}

void Game::moveSnake() {
    // 뱀을 움직이고 이동 데이터를 서버에 전송
    bool move = snake1.moveSnake(board1);

    string msg = tools.createJsonData(snake1, "move");
    client.sendData(msg);

    // 뱀이 움직일 수 없는 경우, 게임 종료 표시 및 서버에 알림
    if (!move) {
        isWin = false;
        msg = tools.createJsonData(snake1, "end");
        client.sendData(msg);
    }
}
