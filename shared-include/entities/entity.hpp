#pragma once
#include "engine.hpp"

#include <vector>
#include <string>

class IComponent;

class IEntity
{
public:
	virtual ~IEntity() = default;

    virtual void AddComponent(std::string name, IComponent* component) = 0;
    virtual void DeleteComponent(std::string name) = 0;
    virtual IComponent* GetComponent(std::string name) = 0;

    virtual std::string& GetName() = 0;
    virtual void SetName(std::string name) = 0;

    virtual void SetParent(IEntity* parent) = 0;

    virtual IEntity* GetParent() = 0;

    virtual std::vector<IEntity*>& GetChildrens() = 0;

    virtual void AddChildren(IEntity* entity) = 0;
    virtual void RemoveChildren(IEntity* entity) = 0;
};