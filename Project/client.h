#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <thread>

#include "snake.h"
#include "tools.h"

using namespace std;

class Client {
private:
	SOCKET clientSocket;
	WSADATA wsaData;
	SOCKADDR_IN clientAddr;

public:
    bool openClientSocket(HWND hwnd, const char* serverIP, const char* serverPort);
    void closeClientSocket();
    void SendData(const string& msg);
    void makeThread(Snake& snake1, Snake& snake2);
};
