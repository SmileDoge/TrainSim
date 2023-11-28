#pragma once
#include "module.hpp"
#include "entities/component.hpp"

#include <string>

class IComponentFactory : public IModule
{
public:
	virtual ~IComponentFactory() = default;

    virtual void RegisterComponent(std::string name, PROC_CreateComponent createComponent, PROC_DeleteComponent deleteComponent) = 0;

    virtual IComponent* CreateComponent(std::string name) = 0;
    virtual void DeleteComponent(IComponent* component) = 0;
};