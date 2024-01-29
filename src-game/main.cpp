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
    /*
    std::cout << "Engine Date Build = " << date << std::endl;
    std::cout << "Engine Time Build = " << time << std::endl;
    std::cout << "Engine Version " << major << "." << minor << "." << patch << std::endl;
    */
}

int main()
{
    //Создание экземпляра движка
    IEngine* engine = CreateEngine();
    //Получение модуля логирования
    ILogModule* log = engine->GetModule<ILogModule>();

    //Создание экземпляра игры
    TrainSimGame* game = new TrainSimGame(engine);

    //Вывод информации о движке
    ShowEngineInfo(engine, log);

    //Установка игры в движок
    engine->SetGame(game);

    //Инизициализация движка
    TSResult result = engine->Initialize();

    // Проверка результата
    if (result != TS_OK)
    {
        log->LogError("engine->Initialize returns error (%d)", (int)result);
        return -1;
    }

    // Запуск главного цикла движка
    engine->RunLoop();

    // Очищение движка
    FreeEngine();
    
    return 0;
}