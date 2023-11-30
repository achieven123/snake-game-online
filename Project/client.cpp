#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "client.h"

// Ŭ���̾�Ʈ ���� ���� �Լ�
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

// Ŭ���̾�Ʈ ���� �ݱ� �Լ�
void Client::closeClientSocket() {
    closesocket(clientSocket);
    WSACleanup();
}

// ������ ���� �Լ�
void Client::sendData(const string& sendMsg) {
    send(clientSocket, sendMsg.c_str(), sendMsg.size(), 0);
}

/*
* @brief ������ ���� �Լ�
* @param clientSocket Ŭ���̾�Ʈ ����
* @param snake ��� �÷��̾� ����
* @param msg, code, player ���� ����
* @details �����κ��� �����͸� �޾� ��� Snake�� ���� ���� ����(msg, code, player)�� ����
*/
void recvData(SOCKET clientSocket, Snake& snake, string& msg, string& code, int& state) {
    char buffer[PACKET_SIZE];
    Tools tools;

    while (!WSAGetLastError()) {
        ZeroMemory(&buffer, PACKET_SIZE);
        recv(clientSocket, buffer, PACKET_SIZE, 0);
        cout << ">> Server:" << buffer << endl;
        tools.parsingJsonData(buffer, snake, msg, code, state);
    }
}

// ������ ���� �Լ�
void Client::makeThread(Snake& snake, string& msg, string& code, int& state) {
    thread recvThread = thread(recvData, clientSocket, ref(snake), ref(msg), ref(code), ref(state));
    recvThread.detach();
}
