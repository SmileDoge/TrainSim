#pragma once
#include "module.hpp"
#include "entities/component.hpp"

#include <string>

class IComponentFactory : public IModule
{
public:
	virtual ~IComponentFactory() = default;

    virtual void RegisterComponent(IComponentInfo info) = 0;

    virtual std::string& GetComponentName(USHORT id) = 0;
    virtual USHORT GetComponentID(const std::string& name) = 0;

    virtual IComponent* CreateComponent(const std::string& name) = 0;
    virtual IComponent* CreateComponent(USHORT id) = 0;
    virtual void DeleteComponent(IComponent* component) = 0;
};