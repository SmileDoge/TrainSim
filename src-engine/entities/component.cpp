#pragma once

#include "entities/component.hpp"
#include "modules/ccomponentfactory.hpp"

IEntity* IComponent::GetEntity()
{
    return entity;
}

void IComponent::SetEntity(IEntity* entity)
{
    this->entity = entity;
}

std::string& IComponent::GetName()
{
    return g_ComponentFactory->GetComponentName(id);
}

USHORT IComponent::GetID()
{
    return id;
}

void IComponent::SetID(USHORT id)
{
    this->id = id;
}