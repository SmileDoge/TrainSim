#pragma once

class IEngine;

class IGame
{
public:
    virtual ~IGame() = default;

    virtual void Start() = 0;
    virtual void RegisterComponents() = 0;
    virtual void PostStart() = 0;

    virtual void ProcessInput() = 0; // before EarlyUpdate()
    virtual void EarlyUpdate() = 0; // before update all entities
    virtual void Update() = 0; // after update all entities and before late update all entities
    virtual void LateUpdate() = 0; // after late update all entities
    virtual void Shutdown() = 0;

protected:
    IEngine* engine;
};