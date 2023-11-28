#pragma once
#include "entity.hpp"
#include "engine.hpp"

#define DEFINE_COMPONENT_FUNCTIONS(name) \
IComponent* CreateComponent_##name##() { return new name(); } \
void DeleteComponent_##name##(IComponent* component) { delete component; } \

class ENGINE_EXPORT IComponent
{
public:
    virtual ~IComponent() = default;

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void LateUpdate() = 0;
    virtual void OnDestroy() = 0;

    IEntity* GetEntity();

private:
    IEntity* entity;
};

typedef IComponent* (*PROC_CreateComponent)(void);
typedef void (*PROC_DeleteComponent)(IComponent*);


//IComponent* CreateComponent();
//void DeleteComponent(IComponent*);