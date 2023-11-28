#include "cworld.hpp"
#include "entities/centity.hpp"

CWorld::CWorld() : entities()
{

}

CWorld::~CWorld()
{

}

IEntity* CWorld::CreateEntity()
{
    CEntity* entity = new CEntity();

    entities.push_back(entity);

    return entity;
}

void CWorld::DeleteEntity(IEntity* entity)
{
    auto iter = std::find(entities.begin(), entities.end(), entity);
    if (iter == entities.end())
        return;

    entities.erase(iter);

    entity->SetParent(NULL);

    for (auto& children : entity->GetChildrens())
        children->SetParent(NULL);

    delete ((CEntity*)entity);
}

void CWorld::DeleteEntityWithChildrens(IEntity* entity)
{
    auto iter = std::find(entities.begin(), entities.end(), entity);
    if (iter == entities.end())
        return;

    entities.erase(iter);

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