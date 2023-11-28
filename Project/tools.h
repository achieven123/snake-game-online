#pragma once

#include <iostream>
#include <codecvt>
#include "json/json.h"
#include "snake.h"

using namespace std;

class Tools {
public:
    LPCWSTR IntToLPCWSTR(int value);
    const wchar_t* ConvertToWideString(const string& str);
    
    /*
    * @brief Json ������ ����� �Լ�
    * @param snake Json�� ����� ���� ���� Snake ����
    * @param msg ������ ������ �޽���
    * @return ���� Json ���� ������ ��ȯ
    * @details �� Snake ������ ������ msg�� �������� ó���ϵ��� Json ����
    */
    string createJsonData(Snake& snake, const string msg);

	/*
    * @brief Json ������ �Ľ� �Լ�
    * @param recvMsg �����κ��� ���� ������
    * @param snake, msg, code, player �Ľ��� �����ͷ� �ٲ� ������ ����
    * @details
    * Json �����͸� �Ľ��� ��, ������ ������ ����
    * msg�� move�� ������ ��� �÷��̾ ������ ������Ʈ
    */
    void parsingJsonData(const string recvMsg, Snake& snake, string& msg, string& code, int& player);
};