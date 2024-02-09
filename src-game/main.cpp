#include <iostream>

#include "engine.hpp"
#include "world.hpp"
#include "modules/rendermodule.hpp"
#include "modules/componentfactory.hpp"
#include "modules/logmodule.hpp"
#include "cgame.hpp"

void ShowEngineInfo(IEngine* engine, ILogModule* log)
{
    short major, minor, patch;

    char* date, *time, *compiler;

    long version = 0;

    engine->GetBuildInfo(date, time, version, compiler);
    engine->GetEngineVersion(major, minor, patch);

    log->LogInfo("Engine Date Build %s", date);
    log->LogInfo("Engine Time Build %s", time);
    log->LogInfo("Engine Version %d.%d.%d", major, minor, patch);
    log->LogInfo("Engine Compiler %s", compiler);
}

int main()
{
    IEngine* engine = CreateEngine();
    ILogModule* log = engine->GetModule<ILogModule>();

    TrainSimGame* game = new TrainSimGame(engine);

    ShowEngineInfo(engine, log);

    engine->SetGame(game);

    TSResult result = engine->Initialize();

    if (result != TS_OK)
    {
        log->LogError("engine->Initialize returns error (%d)", (int)result);
        return -1;
    }

    engine->RunLoop();

    FreeEngine();
    
    return 0;
}