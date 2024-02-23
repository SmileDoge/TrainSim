#include <iostream>

#include "engine.hpp"
#include "world.hpp"
#include "modules/rendermodule.hpp"
#include "modules/componentfactory.hpp"
#include "modules/logmodule.hpp"
#include "cgame.hpp"

void ShowEngineInfo(IEngine* engine, ILogModule* log)
{
    char* date, *time, *compiler;

    long cpp_version = 0;

    engine->GetBuildInfo(date, time, cpp_version, compiler);
    TSVersion version = engine->GetEngineVersion();

    log->LogInfo("Engine Date Build %s", date);
    log->LogInfo("Engine Time Build %s", time);
    log->LogInfo("Engine Version %d.%d.%d", version.major, version.minor, version.patch);
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