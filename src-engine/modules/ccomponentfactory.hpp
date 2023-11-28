#pragma once
#include "modules/componentfactory.hpp"

#include <map>

typedef struct
{
    PROC_CreateComponent createComponentProc;
    PROC_DeleteComponent deleteComponentProc;
} Component_Info;

class CComponentFactory : public IComponentFactory
{
public:
	CComponentFactory();
	~CComponentFactory();

    virtual void PostInit() {};
    virtual void PreDeinit() {};
    virtual void Update() {};

    virtual void RegisterComponent(std::string name, PROC_CreateComponent createComponent, PROC_DeleteComponent deleteComponent);

    virtual IComponent* CreateComponent(std::string name);
    virtual void DeleteComponent(IComponent* component);
private:
    std::map<std::string, Component_Info> components;
};

extern CComponentFactory* g_ComponentFactory;