#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <thread>
#include "snake.h"
#include "tools.h"

#define PACKET_SIZE 10240

using namespace std;

class Client {
private:
	SOCKET clientSocket;
	WSADATA wsaData;
	SOCKADDR_IN clientAddr;

public:
	/*
	* @brief 서버 연결 함수
	* @param hwnd 윈도우 핸들
	* @param serverIP 연결 서버 아이피
	* @param serverPort 연결 서버 포트
	* @return 서버 연결 성공 시 True 반환
	* @details winsock2 라이브러리에 있는 함수로 소켓 생성 및 서버 연결
	*/
    bool openClientSocket(HWND hwnd, const char* serverIP, const char* serverPort);

	/*
	* @brief 서버 연결 끊기 함수
	* @details 연결했던 서버와의 연결 끊기
	*/
    void closeClientSocket();

	/*
	* @brief 데이터 송신 함수
	* @param msg 서버에 보낼 데이터
	* @details Json 형식으로 서버에 데이터를 보냄
	*/
    void sendData(const string& msg);
	
	/*
	* @brief 스레드 생성 함수
	* @param snake 상대 플레이어 정보
	* @param msg, code, player 게임 정보
	* @details snake, msg, code, player를 참조 형식으로 받아 데이터 수신 함수로 전달
	*/
	void makeThread(Snake& snake, string& msg, string& code, int& state);
};
