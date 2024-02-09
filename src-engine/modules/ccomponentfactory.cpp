#include "modules/ccomponentfactory.hpp"
#include "modules/clogmodule.hpp"

CComponentFactory* g_ComponentFactory = NULL;

std::string UnknownText = "Unknown";
IComponentInfo UnknownComponent = IComponentInfo();

CComponentFactory::CComponentFactory() : components(), mapped_names()
{
    g_ComponentFactory = this;

    components[0] = UnknownComponent;
    mapped_names[UnknownText] = NULL;
}

CComponentFactory::~CComponentFactory()
{

}

void CComponentFactory::RegisterComponent(IComponentInfo info)
{
    if (info.createComponent == NULL)
        return;

    info.id = components.size();

    //g_Log->LogWarn("component: %s, %d, create: %p, delete: %p", info.name.c_str(), info.id, info.createComponent, info.deleteComponent);

    components[info.id] = info;
    mapped_names[info.name] = info.id;
}

std::string& CComponentFactory::GetComponentName(USHORT id)
{
    for (auto& [name, num] : mapped_names)
    {
        if (id == num)
            return (std::string&)name;
    }

    return UnknownText;
}

USHORT CComponentFactory::GetComponentID(const std::string& name)
{
    if (mapped_names.find(name) == mapped_names.end())
        return 0;

    return mapped_names[name];
}

IComponent* CComponentFactory::CreateComponent(USHORT id)
{
    if (id == 0)
        return NULL;

    if (components.find(id) == components.end())
        return NULL;

    IComponentInfo& info = components[id];

    IComponent* component = info.createComponent();

    component->SetID(id);

    component->SetEnabled(true);

    return component;
}

IComponent* CComponentFactory::CreateComponent(const std::string& name)
{
    return CreateComponent(GetComponentID(name));
}

void CComponentFactory::DeleteComponent(IComponent* component)
{
    printf("CComponentFactory::DeleteComponent - TODO\n");
    delete component;
}