#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "client.h"

bool Client::openClientSocket(HWND hwnd, const char* serverIP, const char* serverPort) {
	// 1. Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << ">> WSAStartup() Error" << endl;
		return false;
	}

	// 2. 소켓 생성
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		cout << ">> socket() Error" << endl;
		WSACleanup();
		return false;
	}

	// 3. 서버 주소 구조체 설정
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = inet_addr(serverIP);
	clientAddr.sin_port = htons(atoi(serverPort));

	// 4. 서버 연결
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

void Client::sendData(const string& data) {
	send(clientSocket, data.c_str(), data.size(), 0);
}

/*
* @brief 데이터 수신 함수
* @param clientSocket 클라이언트 소켓
* @param snake 상대 플레이어 정보
* @param msg, code, player 게임 정보
* @details 서버로부터 데이터를 받아 상대 Snake의 값과 게임 정보(msg, code, player)를 설정
*/
void recvData(SOCKET clientSocket, Snake& snake, string& msg, string& code, int& player) {
	char buffer[PACKET_SIZE];
	Tools tools;

	while (!WSAGetLastError()) {
		ZeroMemory(&buffer, PACKET_SIZE);
		recv(clientSocket, buffer, PACKET_SIZE, 0);
		cout << ">> Server:" << buffer << endl;
		tools.parsingJsonData(buffer, snake, msg, code, player);
	}
}

void Client::makeThread(Snake& snake, string& msg, string& code, int& player) {
	thread recvThread = thread(recvData, clientSocket, ref(snake), ref(msg), ref(code), ref(player));
	recvThread.detach();
}
