#pragma once
#include "entities/entity.hpp"

#include <string>
#include <map>

class CEntity : public IEntity
{
public:
    CEntity();
    ~CEntity() override;

    virtual void AddComponent(std::string name, IComponent* component) ;
    virtual void DeleteComponent(std::string name);
    virtual IComponent* GetComponent(std::string name);

    virtual std::string& GetName();
    virtual void SetName(std::string name);

    virtual void SetParent(IEntity* parent);

    virtual IEntity* GetParent();

    virtual std::vector<IEntity*>& GetChildrens();

    virtual void AddChildren(IEntity* entity);
    virtual void RemoveChildren(IEntity* entity);
    
private:
    IEntity* parent;
    std::vector<IEntity*> childrens;

    std::string name;

    std::map<std::string, IComponent*> components;
};