#include "clogmodule.hpp"
#include "engine.hpp"

#include "time.h"

CLogModule* g_Log = NULL;

#ifdef MSVC_COMPILER
#include "Windows.h"
#endif

#undef ERROR
#undef DEBUG

CLogModule::CLogModule()
{
	g_Log = this;

#ifdef MSVC_COMPILER
	HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(handleOut, &consoleMode);
	consoleMode |= 0x0004;
	consoleMode |= 0x0008;
	SetConsoleMode(handleOut, consoleMode);
#endif
}

CLogModule::~CLogModule()
{

}

void CLogModule::LogString(ILogType type, const std::string& string)
{
	time_t rawtime;
	time(&rawtime);
	struct tm* curr_time = localtime(&rawtime);

	char* prefix;

	switch (type)
	{
	case ILogType::INFO:
		prefix = "\x1b[92mINFO\x1b[97m";
		break;
	case ILogType::WARN:
		prefix = "\x1b[93mWARN\x1b[97m";
		break;
	case ILogType::ERROR:
		prefix = "\x1b[91mERROR\x1b[97m";
		break;

	default:
		prefix = "\x1b[96mDEBUG\x1b[97m";
		break;
	}

	printf("\x1b[97m[%02d:%02d:%02d] %s\x1b[97m - %s\x1b[0m\n", curr_time->tm_hour, curr_time->tm_min, curr_time->tm_sec, prefix, string.c_str());
}

void CLogModule::LogInfo(const std::string& format, ...)
{
	char buf[2048];

	va_list args;
	va_start(args, format.c_str());
	vsprintf(buf, format.c_str(), args);
	va_end(args);

	LogString(ILogType::INFO, buf);
}

void CLogModule::LogWarn(const std::string& format, ...)
{
	char buf[2048];

	va_list args;
	va_start(args, format.c_str());
	vsprintf(buf, format.c_str(), args);
	va_end(args);

	LogString(ILogType::WARN, buf);
}

void CLogModule::LogError(const std::string& format, ...)
{
	char buf[2048];

	va_list args;
	va_start(args, format.c_str());
	vsprintf(buf, format.c_str(), args);
	va_end(args);

	LogString(ILogType::ERROR, buf);
}

void CLogModule::LogDebug(const std::string& format, ...)
{
	char buf[2048];

	va_list args;
	va_start(args, format.c_str());
	vsprintf(buf, format.c_str(), args);
	va_end(args);

	LogString(ILogType::DEBUG, buf);
}

/*

	virtual void LogString(ILogType type, std::string string);
	virtual void LogString(ILogType type, char* string);

	virtual void LogInfo(std::string format, ...);
	virtual void LogInfo(char* format, ...);

	virtual void LogWarn(std::string format, ...);
	virtual void LogWarn(char* format, ...);

	virtual void LogError(std::string format, ...);
	virtual void LogError(char* format, ...);

	virtual void LogDebug(std::string format, ...);
	virtual void LogDebug(char* format, ...);
	*/