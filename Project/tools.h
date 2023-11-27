#pragma once

#include <iostream>
#include <codecvt>

#include "json/json.h"
#include "snake.h"

using namespace std;

class Tools {
public:
    const wchar_t* ConvertToWideString(const string& str);
    string CreateJsonData(Snake& snake, const string msg);
    void ParsingJsonData(Snake& snake1, Snake& snake2, const string msg);
};