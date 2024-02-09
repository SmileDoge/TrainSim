#pragma once

#include "entities/component.hpp"

#include "modules/render/light.hpp"

class ILightComponent : public IComponent
{
public:
    virtual ~ILightComponent() = default;

    virtual ILight* GetLight() = 0;
};