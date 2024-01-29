#include "centity.hpp"
#include "entities/component.hpp"
#include "modules/ccomponentfactory.hpp"

#include <algorithm>

CEntity::CEntity() : parent(NULL), components(), childrens(), name("CEntity"), enabled(true)
{
    transform = (ITransform*)CreateComponent("Transform");
}

CEntity::~CEntity()
{
    for (auto& [id, component] : components)
        DeleteComponentCore(component);
}

void CEntity::Update()
{
    for (auto& [_, component] : components)
        component->Update();
}

void CEntity::LateUpdate()
{
    for (auto& [_, component] : components)
        component->LateUpdate();
}

void CEntity::AddChildren(IEntity* children)
{
    childrens.push_back(children);
}

void CEntity::RemoveChildren(IEntity* children)
{
    childrens.erase(std::remove(childrens.begin(), childrens.end(), children), childrens.end());
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

void CEntity::SetName(const std::string& newName)
{
    name = newName;
}

std::string& CEntity::GetName()
{
    return name;
}

IComponent* CEntity::CreateComponent(USHORT id)
{
    IComponent* component = g_ComponentFactory->CreateComponent(id);

    if (component == NULL) return NULL;

    component->SetEntity(this);

    component->Attached(this);

    component->Start();

    components[id] = component;

    return component;
}

IComponent* CEntity::CreateComponent(const std::string& name)
{
    return CreateComponent(g_ComponentFactory->GetComponentID(name));
}

void CEntity::AddComponent(IComponent* component)
{
    if (component == NULL) return;

    if (component->GetID() == 0) return;

    if (component->GetEntity() != NULL) return;

    component->SetEntity(this);

    component->Attached(this);

    component->Start();

    components[component->GetID()] = component;
}

void CEntity::DeleteComponentCore(IComponent* component)
{
    component->Detached(this);

    component->OnDestroy();

    delete component;
}

void CEntity::DeleteComponent(IComponent* component)
{
    components.erase(component->GetID());

    DeleteComponentCore(component);
}

IComponent* CEntity::GetComponentInternal(const std::string& name)
{
    return GetComponentInternal(g_ComponentFactory->GetComponentID(name));
}

IComponent* CEntity::GetComponentInternal(USHORT id)
{
    if (components.find(id) == components.end())
        return NULL;

    return components[id];
}

ITransform* CEntity::GetTransform()
{
    return transform;
}