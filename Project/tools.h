#pragma once

#include <windows.h>
#include <iostream>

#include "json/json.h"
#include "snake.h"

using namespace std;

class Tools {
public:
    string CreateJsonData(Snake& snake, const string msg);
    void ParsingJsonData(Snake& snake1, Snake& snake2, const string msg);
};