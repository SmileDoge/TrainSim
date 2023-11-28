#pragma once
#include "entities/entity.hpp"

#include <vector>

class IWorld
{
public:
    virtual ~IWorld() = default;

    virtual IEntity* CreateEntity() = 0;
    virtual void DeleteEntity(IEntity* entity) = 0;
    virtual void DeleteEntityWithChildrens(IEntity* entity) = 0;

    virtual std::vector<IEntity*>& GetEntities() = 0;
};