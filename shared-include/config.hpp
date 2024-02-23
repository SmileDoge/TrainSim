#pragma once

#include <string>

#include "nlohmann/json.hpp"

class IConfig
{
public:
	virtual ~IConfig() = default;

	virtual std::string GetFilename() = 0;

	virtual std::string GetString(const std::string& key) = 0;
	virtual int GetInteger(const std::string& key) = 0;
	virtual float GetFloat(const std::string& key) = 0;
	virtual bool GetBool(const std::string& key) = 0;
	virtual nlohmann::json::value_t GetType(const std::string& key) = 0;

	virtual nlohmann::json& GetData() = 0;
};