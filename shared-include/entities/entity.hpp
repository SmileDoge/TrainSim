#pragma once
#include "global.hpp"
#include "engine.hpp"

#include <vector>
#include <string>

class IComponent;
class ITransform;

class IEntity
{
public:
	virtual ~IEntity() = default;

    virtual IComponent* CreateComponentInternal(const std::string& name) = 0;
    virtual IComponent* CreateComponentInternal(USHORT id) = 0;

    virtual void AddComponent(IComponent* component) = 0;
    virtual void DeleteComponent(IComponent* component) = 0;

    virtual IComponent* GetComponentInternal(const std::string& name) = 0;
    virtual IComponent* GetComponentInternal(USHORT id) = 0;

    virtual ITransform* GetTransform() = 0;

    virtual void Update() = 0;
    virtual void LateUpdate() = 0;

    virtual std::string& GetName() = 0;
    virtual void SetName(const std::string& name) = 0;

    virtual bool GetEnabled() = 0;
    virtual void SetEnabled(bool enabled) = 0;

    virtual void SetParent(IEntity* parent) = 0;

    virtual IEntity* GetParent() = 0;

    virtual std::vector<IEntity*>& GetChildrens() = 0;

    virtual void AddChildren(IEntity* entity) = 0;
    virtual void RemoveChildren(IEntity* entity) = 0;

    template<class T>
    T* GetComponent();

    template<class T>
    T* CreateComponent();
};

#include <typeinfo>
template<class T>
T* IEntity::GetComponent()
{
    auto name = std::string(typeid(T).name()).substr(6);

    IComponent* component = GetComponentInternal(name);

    if (component == NULL)
        component = GetComponentInternal(name.substr(1));

    return (T*)(void*)component;
}

template<class T>
T* IEntity::CreateComponent()
{
    auto name = std::string(typeid(T).name()).substr(6);

    IComponent* component = CreateComponentInternal(name);

    if (component == NULL)
        component = CreateComponentInternal(name.substr(1));

    return (T*)(void*)component;
}