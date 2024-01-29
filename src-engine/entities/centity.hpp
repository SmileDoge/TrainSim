#pragma once
#include "entities/entity.hpp"

#include <string>
#include <map>

class CEntity : public IEntity
{
public:
    CEntity();
    ~CEntity() override;

    virtual IComponent* CreateComponent(const std::string& name);
    virtual IComponent* CreateComponent(USHORT id);

    virtual void AddComponent(IComponent* component);
    virtual void DeleteComponent(IComponent* component);

    virtual IComponent* GetComponentInternal(const std::string& name);
    virtual IComponent* GetComponentInternal(USHORT id);

    virtual ITransform* GetTransform();

    virtual void Update();
    virtual void LateUpdate();

    virtual std::string& GetName();
    virtual void SetName(const std::string& name);

    virtual bool GetEnabled() { return enabled; };
    virtual void SetEnabled(bool enabled) { this->enabled = enabled; };

    virtual void SetParent(IEntity* parent);

    virtual IEntity* GetParent();

    virtual std::vector<IEntity*>& GetChildrens();

    virtual void AddChildren(IEntity* entity);
    virtual void RemoveChildren(IEntity* entity);
    
private:
    void DeleteComponentCore(IComponent* component);

    ITransform* transform;

    IEntity* parent;
    std::vector<IEntity*> childrens;

    std::string name;

    std::map<USHORT, IComponent*> components;

    bool enabled;
};