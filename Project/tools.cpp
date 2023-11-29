#include "tools.h"

wstring Tools::stringToWString(const string& str) {
	int len;
	int slength = (int)str.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
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

void Tools::parsingJsonData(const string recvMsg, Snake& snake, string& msg, string& code, int& state) {
	Json::Value json;
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	
	JSONCPP_STRING err;

	bool parsingSuccessful = reader->parse(recvMsg.c_str(), recvMsg.c_str() + recvMsg.size(), &json, &err);
	delete reader;

	if (parsingSuccessful) {
		msg = json["msg"].asString();
		code = json["code"].asString();
		
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

		if (msg == "ready") {
			state = 4;
		}
		else if (msg == "start") {
			state = 7;
		}
		else if (msg == "end") {
			state = 9;
		}
		else if (msg == "move") {
			snake.setScore(score);
			snake.setLength(length);
			snake.setFood(food);
			snake.setLocation(location);
		}
	}
}
