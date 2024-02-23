#include "cconfig.hpp"

#include "modules/cfilesystem.hpp"

CConfig::CConfig(const std::string& path)
{
	filename = IFileSystem::GetFileName(path);

	if (!g_FileSystem->FileExists(path))
		g_FileSystem->WriteFileString(path, "{\"test\": 123}");

	std::string json_text = g_FileSystem->ReadFileString(path);

	data = nlohmann::json::parse(json_text);
}

CConfig::~CConfig()
{
}

std::string CConfig::GetFilename()
{
	return filename;
}

std::string CConfig::GetString(const std::string& key)
{
	auto& str = data.at(key);

	if (str.is_string())
		return str.get<std::string>();

	return std::string();
}

int CConfig::GetInteger(const std::string& key)
{
	auto& integer = data.at(key);

	if (integer.is_number_integer() || integer.is_number_unsigned())
		return integer.get<int>();

	return 0;
}

float CConfig::GetFloat(const std::string& key)
{
	auto& number = data.at(key);

	if (number.is_number())
		return number.get<float>();

	return 0.f;
}

bool CConfig::GetBool(const std::string& key)
{
	auto& boolean = data.at(key);

	if (boolean.is_boolean())
		return boolean.get<bool>();

	return false;
}

nlohmann::json::value_t CConfig::GetType(const std::string& key)
{
	return nlohmann::json::value_t();
}

nlohmann::json& CConfig::GetData()
{
	return data;
}
