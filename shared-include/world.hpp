#pragma once

#include "entities/entity.hpp"

#include "glm/glm.hpp"

#include <vector>

class IWorld
{
public:
    virtual ~IWorld() = default;

    virtual IEntity* CreateEntity() = 0;
    virtual void DeleteEntity(IEntity* entity) = 0;
    virtual void DeleteEntityWithChildrens(IEntity* entity) = 0;

    virtual std::vector<IEntity*>& GetEntities() = 0;

    virtual void SetSunDirection(glm::vec3 dir) = 0;
    virtual glm::vec3& GetSunDirection() = 0;

    virtual void SetSunColor(glm::vec3 color) = 0;
    virtual glm::vec3& GetSunColor() = 0;

    virtual void SetAmbientColor(glm::vec3 color) = 0;
    virtual glm::vec3& GetAmbientColor() = 0;
};