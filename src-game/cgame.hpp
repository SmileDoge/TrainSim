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

extern TrainSimGame* g_Game;
extern ILogModule* g_Log;

class ExampleGameComponent : public IComponent
{
public:
    virtual void Start() { g_Log->LogInfo("ExampleGameComponent Start()"); };
    virtual void Update() { };
    virtual void LateUpdate() { };
    virtual void OnDestroy() { g_Log->LogInfo("ExampleGameComponent OnDestroy()"); };

    virtual void Attached(IEntity* ent) { g_Log->LogInfo("ExampleGameComponent Attached to %p", ent); };
    virtual void Detached(IEntity* ent) { g_Log->LogInfo("ExampleGameComponent Detached to %p", ent); };

};
