#pragma once

#include "engine.hpp"
#include "cworld.hpp"
#include "cconfig.hpp"

#include <string>
#include <map>
#include <chrono>

#define ENGINE_BUILDED_DATE __DATE__
#define ENGINE_BUILDED_TIME __TIME__
#define ENGINE_BUILDED_CPP_VER __cplusplus

#define ENGINE_VERSION "0.1.0"
#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 1
#define ENGINE_VERSION_PATCH 0

class CEngine : public IEngine
{
public:
    CEngine();
    ~CEngine();

    virtual TSResult Initialize();

    virtual void SetGame(IGame* game);
    virtual IGame* GetGame();

    virtual void Update();

    virtual IModule* GetModuleInternal(const std::string& name);
    virtual void AddModule(const std::string& name, IModule* module);
    virtual void DeleteModule(const std::string& name);

    virtual IWorld* GetWorld();

    virtual IConfig* GetEngineConfig();
    virtual IConfig* GetGameConfig();

    virtual void GetBuildInfo(char*& date, char*& time, long& cppVersion, char*& compiler);

    virtual const char* GetEngineVersionString() { return ENGINE_VERSION; };
    virtual TSVersion GetEngineVersion();

    virtual void RunLoop();

    virtual void SetFPS(float fps);
    virtual float GetFPS();

    virtual float GetDeltaTime();
    virtual double GetCurTime();
    virtual double GetSysTime();

    virtual int GetCurrentThreadID();
private:
    std::map<std::string, IModule*> modules;

    double prev_frame_time;
    double current_time;

    float delta_time;
    float target_frametime;

    IGame* game;
    CWorld* world;

    CConfig* engine_config;
    CConfig* game_config;
};

extern CEngine* g_Engine;