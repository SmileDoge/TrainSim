#include "modules/ccomponentfactory.hpp"

CComponentFactory* g_ComponentFactory = NULL;

CComponentFactory::CComponentFactory() : components()
{
    g_ComponentFactory = this;
}

CComponentFactory::~CComponentFactory()
{

}

void CComponentFactory::RegisterComponent(std::string name, PROC_CreateComponent createComponent, PROC_DeleteComponent deleteComponent)
{
    if (createComponent == NULL)
        return;

    Component_Info info;

    info.createComponentProc = createComponent;
    info.deleteComponentProc = deleteComponent;

    components[name] = info;
}

IComponent* CComponentFactory::CreateComponent(std::string name)
{
    if (components.find(name) == components.end())
        return NULL;

    Component_Info& info = components[name];

    IComponent* component = info.createComponentProc();

    return component;
}

void CComponentFactory::DeleteComponent(IComponent* component)
{
    printf("CComponentFactory::DeleteComponent - TODO\n");
    delete component;
}