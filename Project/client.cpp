#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define PACKET_SIZE 10240

#include "client.h"

void RecvData(SOCKET clientSocket, Snake& snake1, Snake& snake2) {
	char buffer[PACKET_SIZE] = {};
	Tools tools;

	while (!WSAGetLastError()) {
		ZeroMemory(&buffer, PACKET_SIZE);
		recv(clientSocket, buffer, PACKET_SIZE, 0);
		tools.ParsingJsonData(snake1, snake2, buffer);
	}
}

bool Client::openClientSocket(HWND hwnd, const char* serverIP, const char* serverPort) {
	// 1. Winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << ">> WSAStartup() Error" << endl;
		return false;
	}

	// 2. ���� ����
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		cout << ">> socket() Error" << endl;
		WSACleanup();
		return false;
	}

	// 3. ���� �ּ� ����ü ����
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr(serverIP);
	clientAddr.sin_port = htons(atoi(serverPort));

	// 4. ���� ����
	if (connect(clientSocket, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
		cout << ">> connect() Error" << endl;
		closesocket(clientSocket);
		WSACleanup();
		return false;
	}
	
	cout << ">> Server Connect!" << endl;
	return true;
}

void Client::closeClientSocket() {
	closesocket(clientSocket);
	WSACleanup();
}

void Client::SendData(const string& msg) {
	send(clientSocket, msg.c_str(), msg.size(), 0);
}

void Client::makeThread(Snake& snake1, Snake& snake2) {
	thread recvThread = thread(RecvData, clientSocket, ref(snake1), ref(snake2));
	recvThread.detach();
}
