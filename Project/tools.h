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
    * @brief Json 데이터 만드는 함수
    * @param snake Json을 만들기 위한 나의 Snake 정보
    * @param msg 서버에 전송할 메시지
    * @return 만든 Json 형태 데이터 반환
    * @details 내 Snake 정보와 각각의 msg을 서버에서 처리하도록 Json 생성
    */
    string createJsonData(Snake& snake, const string msg);

	/*
    * @brief Json 데이터 파싱 함수
    * @param recvMsg 서버로부터 받은 데이터
    * @param snake, msg, code, player 파싱한 데이터로 바꿀 변수의 참조
    * @details
    * Json 데이터를 파싱한 뒤, 각각의 변수에 대입
    * msg가 move일 때에는 상대 플레이어에 데이터 업데이트
    */
    void parsingJsonData(const string recvMsg, Snake& snake, string& msg, string& code, int& player);
};