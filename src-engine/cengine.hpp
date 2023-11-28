#pragma once

#include "engine.hpp"
#include "cworld.hpp"

#include <string>
#include <map>

#define ENGINE_BUILDED_DATE __DATE__
#define ENGINE_BUILDED_TIME __TIME__
#define ENGINE_BUILDED_CPP_VER __cplusplus

#define ENGINE_VERSION_MAJOR 0
#define ENGINE_VERSION_MINOR 1
#define ENGINE_VERSION_PATCH 0

class CEngine : public IEngine
{
public:
    CEngine();
    ~CEngine();

    virtual void Update();

    virtual IModule* GetModule(std::string name);
    virtual void AddModule(std::string name, IModule* module);
    virtual void DeleteModule(std::string name);

    virtual IWorld* GetWorld();

    virtual void GetBuildInfo(char*& date, char*& time, long& cppVersion, char*& compiler);
    virtual void GetEngineVersion(short& major, short& minor, short& patch);

    virtual void RunLoop();

private:
    std::map<std::string, IModule*> modules;

    CWorld* world;
};

extern CEngine* g_Engine;