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

    virtual void SetSunDirection(glm::vec3 dir);
    virtual glm::vec3& GetSunDirection();

    virtual void SetSunColor(glm::vec3 color);
    virtual glm::vec3& GetSunColor();

    virtual void SetAmbientColor(glm::vec3 color);
    virtual glm::vec3& GetAmbientColor();

    void UpdateEntities();
    void LateUpdateEntities();
private:
    std::vector<IEntity*> entities;

    glm::vec3 sun_direction, sun_color;

    glm::vec3 ambient_color;
};