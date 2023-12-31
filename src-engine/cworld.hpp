#pragma once

#include "world.hpp"

class CWorld : public IWorld
{
public:
	CWorld();
	~CWorld() override;

    virtual IEntity* CreateEntity();
    virtual void DeleteEntity(IEntity* entity);
    virtual void DeleteEntityWithChildrens(IEntity* entity);

    virtual std::vector<IEntity*>& GetEntities();
private:
    std::vector<IEntity*> entities;
};