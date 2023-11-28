#include "tools.h"

const wchar_t* Tools::ConvertToWideString(const string& str) {
    static wstring_convert<codecvt_utf8<wchar_t>> converter;
    static wstring wideString = converter.from_bytes(str);
	return wideString.c_str();
}

LPCWSTR IntToLPCWSTR(int value) {
	wchar_t buffer[20]; // 충분한 크기의 버퍼 할당
	swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%d", value); // 정수를 유니코드 문자열로 변환

	return buffer;
}

string Tools::createJsonData(Snake& snake, const string msg) {
	Json::Value json;
	Json::Value food(Json::arrayValue);
	Json::Value location(Json::arrayValue);
	Json::StreamWriterBuilder builder;

	int score = snake.getScore();
	int length = snake.getLength();

	for (const auto& point : snake.getLocation()) {
		Json::Value pointValue(Json::arrayValue);
		pointValue.append(point.first);
		pointValue.append(point.second);
		location.append(pointValue);
	}

	for (const auto& point : snake.getFood()) {
		Json::Value pointValue(Json::arrayValue);
		pointValue.append(point.first);
		pointValue.append(point.second);
		food.append(pointValue);
	}

	json["msg"] = msg;
	json["score"] = score;
	json["length"] = length;
	json["food"] = food;
	json["location"] = location;

	return Json::writeString(builder, json);
}

void Tools::parsingJsonData(const string recvMsg, Snake& snake, string& msg, string& code, int& player) {
	Json::Value json;
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	
	JSONCPP_STRING err;

	bool parsingSuccessful = reader->parse(recvMsg.c_str(), recvMsg.c_str() + recvMsg.size(), &json, &err);
	delete reader;

	if (parsingSuccessful) {
		msg = json["msg"].asString();
		code = json["code"].asString();
		player = json["player"].asInt();
		
		int score = json["score"].asInt();
		int length = json["length"].asInt();

		vector<pair<int, int>> food;
		vector<pair<int, int>> location;
		
		for (const auto& pair : json["food"]) {
			int x = pair[0].asInt();
			int y = pair[1].asInt();
			food.emplace_back(x, y);
		}
		
		for (const auto& pair : json["location"]) {
			int x = pair[0].asInt();
			int y = pair[1].asInt();
			location.emplace_back(x, y);
		}

		if (msg == "move") {
			snake.setScore(score);
			snake.setLength(length);
			snake.setFood(food);
			snake.setLocation(location);
		}
	}
}
