#pragma once

#include "entities/component.hpp"

IEntity* IComponent::GetEntity()
{
    return entity;
}