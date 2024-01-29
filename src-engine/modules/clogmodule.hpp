#pragma once
#include "modules/logmodule.hpp"

class CLogModule : public ILogModule
{
public:
    CLogModule();
    ~CLogModule() override;

    virtual void PostInit() {};
    virtual void PreDeinit() {};
    virtual void Update() {};

	virtual void LogString(ILogType type, const std::string& string);

	virtual void LogInfo(const std::string& format, ...);
	virtual void LogWarn(const std::string& format, ...);
	virtual void LogError(const std::string& format, ...);
	virtual void LogDebug(const std::string& format, ...);
};

extern CLogModule* g_Log;