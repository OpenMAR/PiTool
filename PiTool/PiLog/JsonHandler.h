#pragma once
#include <string>
#include "json.h"

class JsonCommonHandler
{
private:
	JsonCommonHandler() {}
	~JsonCommonHandler() {}
public:
	static bool ReadJsonObject(std::string path, Json::Value& object);
	static bool ReadJsonObject(std::wstring path,Json::Value& object);
	static bool ReadJsonObject(std::string path, const std::wstring key, Json::Value& object);
	static bool ReadJsonObject(std::wstring path, const std::wstring key, Json::Value& object);
	static bool ReadJsonObject(Json::Value root, const std::string key, Json::Value& object);
	static bool ReadJsonObject(Json::Value root, const std::wstring key, Json::Value& object);
	static bool ReadJsonValue(std::string path, const std::wstring key, std::vector<std::wstring>& value);
	static bool ReadJsonValue(std::wstring path, const std::wstring key, std::vector<std::wstring>& value);
	static bool ReadJsonValue(Json::Value object, const std::string key, std::vector<std::wstring>& value);
	static bool ReadJsonValue(Json::Value object, const std::wstring key, std::vector<std::wstring>& value);
	static bool ReadJsonValue(std::wstring path, const std::wstring key, std::wstring& value);
	static bool ReadJsonValue(Json::Value object, const std::wstring key, std::wstring& value);
	static bool ReadJsonValue(std::wstring path, const std::wstring key, bool& value);
	static bool ReadJsonValue(Json::Value object, const std::wstring key, bool& value);
	static bool ReadJsonValue(std::wstring path, const std::wstring key, int& value);
	static bool ReadJsonValue(Json::Value object, const std::wstring key, int& value);
	static bool ReadJsonValue(std::wstring path, const std::wstring key, double& value);
	static bool ReadJsonValue(Json::Value object, const std::wstring key, double& value);
	static bool WriteJsonValue(std::wstring path, Json::Value object);
	static bool CoverJsonValue(std::wstring path, const std::wstring key, Json::Value object);
	static bool CoverJsonValue(Json::Value& root, const std::wstring key, Json::Value object);
	static bool CoverJsonValue(std::wstring path, const std::wstring key, std::vector<std::wstring> value);
	static bool CoverJsonValue(Json::Value& object, const std::wstring key, std::vector<std::wstring> value);
	static bool CoverJsonValue(std::wstring path, const std::wstring key, std::wstring value);
	static bool CoverJsonValue(Json::Value& object, const std::wstring key, std::wstring value);
	static bool CoverJsonValue(std::wstring path, const std::wstring key, std::string value);
	static bool CoverJsonValue(Json::Value& object, const std::wstring key, std::string value);
	static bool CoverJsonValue(std::wstring path, const std::wstring key, bool value);
	static bool CoverJsonValue(Json::Value& object, const std::wstring key, bool value);
	static bool CoverJsonValue(std::wstring path, const std::wstring key, int value);
	static bool CoverJsonValue(Json::Value& object, const std::wstring key, int value);
	static bool CoverJsonValue(std::wstring path, const std::wstring key, double value);
	static bool CoverJsonValue(Json::Value& object, const std::wstring key, double value);
};
