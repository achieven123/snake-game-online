#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define PACKET_SIZE 10240

#include "client.h"

void RecvData(SOCKET clientSocket, Snake& snake1, Snake& snake2) {
	char buffer[PACKET_SIZE] = {};
	Tools tools;

	while (!WSAGetLastError()) {
		ZeroMemory(&buffer, PACKET_SIZE);
		recv(clientSocket, buffer, PACKET_SIZE, 0);
		cout << ">> Server: " << buffer << endl;
		tools.ParsingJsonData(snake1, snake2, buffer);
	}
}

void Client::openClientSocket(HWND hwnd, const char* serverIP, const char* serverPort) {
	// 1. Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBox(hwnd, TEXT("WSAStartup() Error"), TEXT("Error"), MB_ICONERROR);
		return;
	}

	// 2. 소켓 생성
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		MessageBox(hwnd, TEXT("socket() Error"), TEXT("Error"), MB_ICONERROR);
		WSACleanup();
		return;
	}

	// 3. 서버 주소 구조체 설정
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr(serverIP);
	clientAddr.sin_port = htons(atoi(serverPort));

	// 4. 서버 연결
	if (connect(clientSocket, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
		MessageBox(hwnd, TEXT("connect() Error"), TEXT("Error"), MB_ICONERROR);
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

	MessageBox(hwnd, TEXT("연결 성공"), TEXT("Success"), MB_ICONINFORMATION);
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
