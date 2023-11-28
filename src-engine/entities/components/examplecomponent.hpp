#pragma once
#include "entities/component.hpp"

class ExampleComponent : public IComponent
{
public:
    virtual void Start() { printf("ExampleComponent Start!\n"); };
    virtual void Update() { };
    virtual void LateUpdate() { };
    virtual void OnDestroy() { printf("ExampleComponent OnDestroy!\n"); };

};

DEFINE_COMPONENT_FUNCTIONS(ExampleComponent)
/*
IComponent* CreateComponent_ExampleComponent()
{
    return new ExampleComponent();
}

void DeleteComponent_ExampleComponent(IComponent* component)
{
    delete component;
}
*/