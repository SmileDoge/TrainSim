#pragma once

#include "entities/components/lightcomponent.hpp"

#include "modules/render/clight.hpp"

class CLightComponent : public ILightComponent
{
public:
    CLightComponent();
    ~CLightComponent() override;

    virtual void Update();

    virtual ILight* GetLight();

private:
    ILight* light;
};
