#pragma once
#include "module.hpp"

#include <string>

#undef ERROR
#undef DEBUG

enum class ILogType
{
	INFO,
	WARN,
	ERROR,
	DEBUG,
};

class ILogModule : public IModule
{
public:
	virtual ~ILogModule() = default;

	virtual void LogString(ILogType type, const std::string& string) = 0;

	virtual void LogInfo(const std::string& format, ...) = 0;
	virtual void LogWarn(const std::string& format, ...) = 0;
	virtual void LogError(const std::string& format, ...) = 0;
	virtual void LogDebug(const std::string& format, ...) = 0;
};
