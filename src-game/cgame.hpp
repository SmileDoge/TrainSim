#pragma once

#include "game.hpp"
#include "entities/component.hpp"
#include "modules/logmodule.hpp"

class TrainSimGame : public IGame
{
public:
    TrainSimGame(IEngine* engine);
    ~TrainSimGame() override;

    virtual void Start();
    virtual void RegisterComponents();
    virtual void PostStart();

    virtual void ProcessInput(); // before EarlyUpdate()
    virtual void EarlyUpdate(); // before update all entities
    virtual void Update(); // after update all entities and before late update all entities
    virtual void LateUpdate(); // after late update all entities
    virtual void Shutdown();

private:
    float fps;
};

extern IEngine* g_Engine;
extern TrainSimGame* g_Game;
extern ILogModule* g_Log;

class TestComponent : public IComponent
{
public:
    virtual void Update();

    void SetIndex(int index);
private:
    int index;
};
