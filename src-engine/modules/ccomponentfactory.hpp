#pragma once
#include "modules/componentfactory.hpp"

#include <map>

class CComponentFactory : public IComponentFactory
{
public:
	CComponentFactory();
	~CComponentFactory();

    virtual void PostInit() {};
    virtual void PreDeinit() {};
    virtual void Update() {};

    virtual void RegisterComponent(IComponentInfo info);

    virtual std::string& GetComponentName(USHORT id);
    virtual USHORT GetComponentID(const std::string& name);

    virtual IComponent* CreateComponent(USHORT id);
    virtual IComponent* CreateComponent(const std::string& name);
    virtual void DeleteComponent(IComponent* component);
private:
    std::map<std::string, USHORT> mapped_names;

    std::map<USHORT, IComponentInfo> components;
};

extern CComponentFactory* g_ComponentFactory;