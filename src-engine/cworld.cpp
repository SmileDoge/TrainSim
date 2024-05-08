#include "cworld.hpp"
#include "entities/centity.hpp"
#include "modules/clogmodule.hpp"

CWorld::CWorld() : entities(), sun_direction(0, -1, 0), sun_color(1, 0.9, 0.9), ambient_color(0.2f)
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

void CWorld::DeleteEntities()
{
    for (auto& entity : entities_to_delete)
        delete entity;

    entities_to_delete.clear();
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

    entities_to_delete.push_back(entity);
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

    entities_to_delete.push_back(entity);
}

std::vector<IEntity*>& CWorld::GetEntities()
{
    return entities;
}

void CWorld::SetSunDirection(glm::vec3 dir)
{
    sun_direction = glm::normalize(dir);
}

glm::vec3& CWorld::GetSunDirection()
{
    return sun_direction;
}

void CWorld::SetSunColor(glm::vec3 color)
{
    sun_color = color;
}

glm::vec3& CWorld::GetSunColor()
{
    return sun_color;
}

void CWorld::SetAmbientColor(glm::vec3 color)
{
    ambient_color = color;
}

glm::vec3& CWorld::GetAmbientColor()
{
    return ambient_color;
}
