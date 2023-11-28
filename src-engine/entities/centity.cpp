#include "centity.hpp"
#include "entities/component.hpp"

#include <algorithm>

#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)

CEntity::CEntity() : parent(NULL), components(), childrens(), name(PPCAT("CEntity_", __FILE__))
{

}

CEntity::~CEntity()
{

}

void CEntity::AddChildren(IEntity* children)
{
    childrens.push_back(children);
}

void CEntity::RemoveChildren(IEntity* children)
{
    childrens.erase(std::find(childrens.begin(), childrens.end(), children));
}

std::vector<IEntity*>& CEntity::GetChildrens()
{
    return childrens;
}

void CEntity::SetParent(IEntity* newParent)
{
    if (newParent == NULL && parent != NULL) // Delete parent
    {
        parent->RemoveChildren(this);

        parent = NULL;
    }
    else if (newParent != NULL && parent == NULL)
    {
        newParent->AddChildren(this);

        parent = newParent;
    }
    else if (newParent != NULL && parent != NULL)
    {
        parent->RemoveChildren(this);

        newParent->AddChildren(this);

        parent = newParent;
    }
}

IEntity* CEntity::GetParent()
{
    return parent;
}

void CEntity::SetName(std::string newName)
{
    name = newName;
}

std::string& CEntity::GetName()
{
    return name;
}

void CEntity::AddComponent(std::string name, IComponent* component)
{
    if (component == NULL) return;

    if (components.find(name) != components.end())
        DeleteComponent(name);

    components[name] = component;

    component->Start();
}

void CEntity::DeleteComponent(std::string name)
{
    IComponent* component = components[name];

    component->OnDestroy();

    components.erase(name);

    delete component;
}

IComponent* CEntity::GetComponent(std::string name)
{
    if (components.find(name) == components.end())
        return NULL;

    return components[name];
}


/*

    virtual void AddComponent(char* name, IComponent* component) ;
    virtual void RemoveComponent(char* name);
    virtual IComponent* GetComponent(char* name);
    */