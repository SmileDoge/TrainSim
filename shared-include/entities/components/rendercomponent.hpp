#pragma once

#include "entities/component.hpp"

#include "modules/render/mesh.hpp"
#include "modules/render/material.hpp"

class IRenderComponent : public IComponent
{
public:
    virtual ~IRenderComponent() = default;

    virtual void SetMesh(IMesh* mesh) = 0;
    virtual IMesh* GetMesh() = 0;

    virtual void SetMaterial(IMaterial* material) = 0;
    virtual IMaterial* GetMaterial() = 0;
};