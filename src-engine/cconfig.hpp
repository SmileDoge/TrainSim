#pragma once

#include "config.hpp"

class CConfig : public IConfig
{
public:
	CConfig(const std::string& path);
	~CConfig() override;

	virtual std::string GetFilename();

	virtual std::string GetString(const std::string& key);
	virtual int GetInteger(const std::string& key);
	virtual float GetFloat(const std::string& key);
	virtual bool GetBool(const std::string& key);
	virtual nlohmann::json::value_t GetType(const std::string& key);

	virtual nlohmann::json& GetData();

private:
	std::string filename;

	nlohmann::json data;
};