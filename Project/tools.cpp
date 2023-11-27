#include "tools.h"

const wchar_t* Tools::ConvertToWideString(const string& str) {
    static wstring_convert<codecvt_utf8<wchar_t>> converter;
    static wstring wideString = converter.from_bytes(str);
	return wideString.c_str();
}

string Tools::CreateJsonData(Snake& snake, const string msg) {
	Json::Value json;
	Json::Value food(Json::arrayValue);
	Json::Value location(Json::arrayValue);
	Json::StreamWriterBuilder builder;

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
	json["code"] = snake.getCode();
	json["score"] = snake.getScore();
	json["length"] = snake.getLength();
	json["food"] = food;
	json["location"] = location;

	return Json::writeString(builder, json);
}

void Tools::ParsingJsonData(Snake& snake1, Snake& snake2, const string recvMsg) {
	Json::Value json;
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	
	JSONCPP_STRING err;

	bool parsingSuccessful = reader->parse(recvMsg.c_str(), recvMsg.c_str() + recvMsg.size(), &json, &err);
	delete reader;

	if (parsingSuccessful) {
		string msg = json["msg"].asString();
		string code = json["code"].asString();
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

		if (msg == "create room!" || msg == "join room!" || msg == "join error!") {
			snake1.setMsg(msg);
			snake1.setCode(code);
		}
		else if (msg == "snake is moving!") {
			snake2.setScore(score);
			snake2.setLength(length);
			snake2.setFood(food);
			snake2.setLocation(location);
		}
	}
	else {
		cout << "Json parsing error: " << err << endl;
	}
}
