#include "cworld.hpp"
#include "entities/centity.hpp"
#include "modules/clogmodule.hpp"

CWorld::CWorld() : entities()
{

}

CWorld::~CWorld()
{
    for (auto& entity : entities)
        delete ((CEntity*)entity);
    
    entities.clear();
}

void CWorld::UpdateEntities()
{
    for (auto& entity : entities)
        if (entity->GetEnabled())
            entity->Update();
}

void CWorld::LateUpdateEntities()
{
    for (auto& entity : entities)
        if (entity->GetEnabled())
            entity->LateUpdate();
}

IEntity* CWorld::CreateEntity()
{
    CEntity* entity = new CEntity();

    entities.push_back(entity);

    return entity;
}

void CWorld::DeleteEntity(IEntity* entity)
{
    auto iter = std::remove(entities.begin(), entities.end(), entity);
    if (iter == entities.end())
        return;

    entities.erase(iter, entities.end());

    entity->SetParent(NULL);

    for (auto& children : entity->GetChildrens())
        children->SetParent(NULL);

    delete ((CEntity*)entity);
}

void CWorld::DeleteEntityWithChildrens(IEntity* entity)
{
    auto iter = std::remove(entities.begin(), entities.end(), entity);
    if (iter == entities.end())
        return;

    entities.erase(iter, entities.end());

    entity->SetParent(NULL);

    for (auto& val : entity->GetChildrens())
        DeleteEntityWithChildrens(val);

    delete ((CEntity*)entity);
}

std::vector<IEntity*>& CWorld::GetEntities()
{
    return entities;
}

/*

    virtual IEntity* CreateEntity();
    virtual void DeleteEntity(IEntity* entity);
    virtual void DeleteEntityWithChildrens(IEntity* entity);

    virtual std::vector<IEntity*>& GetEntities();*/