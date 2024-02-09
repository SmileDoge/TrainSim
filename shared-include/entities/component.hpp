#pragma once
#include "global.hpp"
#include "entity.hpp"
#include "engine.hpp"

#define DEFINE_COMPONENT_FUNCTIONS_CLASSNAME(name, class_name) \
IComponent* CreateComponent_##name##() { return new class_name(); } \
void DeleteComponent_##name##(IComponent* component) { delete component; } \

#define DEFINE_COMPONENT_FUNCTIONS(name) DEFINE_COMPONENT_FUNCTIONS_CLASSNAME(name, name)
#define DEFINE_CCOMPONENT_FUNCTIONS(name) DEFINE_COMPONENT_FUNCTIONS_CLASSNAME(name, C##name)

enum class IComponentStandarts : USHORT
{
    Unknown = 0,
    Transform,
    RenderComponent,
    LightComponent,
};

#define GET_COMPONENT_ID(enumid) (USHORT)enumid

#define DEFINE_INFO() IComponentInfo info;

#define REGISTER_COMPONENT(factory, component) \
info.name = #component; \
info.createComponent = CreateComponent_##component; \
info.deleteComponent = DeleteComponent_##component; \
factory->RegisterComponent(info); \

#define REGISTER_COMPONENT_ENGINE(component) REGISTER_COMPONENT(g_ComponentFactory, component)

class ENGINE_EXPORT IComponent
{
public:
    virtual ~IComponent() = default;

    virtual void Attached(IEntity* newEntity) {};
    virtual void Detached(IEntity* prevEntity) {};

    virtual void Start() {};
    virtual void Update() {};
    virtual void LateUpdate() {};
    virtual void OnDestroy() {};

    IEntity* GetEntity();
    void SetEntity(IEntity* entity);

    bool GetEnabled();
    void SetEnabled(bool enabled);

    USHORT GetID();
    void SetID(USHORT id);
    std::string& GetName();

private:
    USHORT id;
    bool enabled;

    IEntity* entity;
};

typedef IComponent* (*PROC_CreateComponent)(void);
typedef void (*PROC_DeleteComponent)(IComponent*);

typedef struct IComponentInfo_t
{
    std::string name;
    USHORT id;
    PROC_CreateComponent createComponent;
    PROC_DeleteComponent deleteComponent;
} IComponentInfo;