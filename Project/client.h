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
	* @brief ���� ���� �Լ�
	* @param hwnd ������ �ڵ�
	* @param serverIP ���� ���� ������
	* @param serverPort ���� ���� ��Ʈ
	* @return ���� ���� ���� �� True ��ȯ
	* @details winsock2 ���̺귯���� �ִ� �Լ��� ���� ���� �� ���� ����
	*/
    bool openClientSocket(HWND hwnd, const char* serverIP, const char* serverPort);

	/*
	* @brief ���� ���� ���� �Լ�
	* @details �����ߴ� �������� ���� ����
	*/
    void closeClientSocket();

	/*
	* @brief ������ �۽� �Լ�
	* @param msg ������ ���� ������
	* @details Json �������� ������ �����͸� ����
	*/
    void sendData(const string& msg);
	
	/*
	* @brief ������ ���� �Լ�
	* @param snake ��� �÷��̾� ����
	* @param msg, code, player ���� ����
	* @details snake, msg, code, player�� ���� �������� �޾� ������ ���� �Լ��� ����
	*/
	void makeThread(Snake& snake, string& msg, string& code, int& state);
};
