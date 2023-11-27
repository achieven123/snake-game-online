#include "tools.h"

//wstring s2ws(const std::string& str) {
//	int slen = (int)str.length() + 1;
//	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slen, 0, 0);
//	wchar_t* buf = new wchar_t[len];
//
//	MultiByteToWideChar(CP_ACP, 0, str.c_str(), slen, buf, len);
//	wstring res(buf);
//	delete[] buf;
//	return res;
//}

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
	//json["code"] = snake.getCode();
	//json["score"] = snake.getScore();
	json["length"] = snake.getLength();
	//json["dx"] = snake.getDx();
	//json["dy"] = snake.getDy();
	json["food"] = food;
	json["location"] = location;

	return Json::writeString(builder, json);
}

void Tools::ParsingJsonData(Snake& snake1, Snake& snake2, const string msg) {
	Json::Value json;
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	
	JSONCPP_STRING err;

	bool parsingSuccessful = reader->parse(msg.c_str(), msg.c_str() + msg.size(), &json, &err);
	delete reader;

	if (parsingSuccessful) {
		string msg = json["msg"].asString();
		string code = json["code"].asString();
		int score = json["score"].asInt();
		int length = json["length"].asInt();
		int dx = json["dx"].asInt();
		int dy = json["dy"].asInt();

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

		if (msg == "Create room!" || msg == "Join room!") {
			snake1.setCode(code);
		}
		else {
			snake2.setLength(length);
			snake2.setFood(food);
			snake2.setLocation(location);
		}
	}
	else {
		cout << "Json parsing error: " << err << endl;
	}
}
