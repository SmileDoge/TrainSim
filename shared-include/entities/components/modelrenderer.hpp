#pragma once

#include "entities/component.hpp"

#include "resources/modelresource.hpp"

class IModelRenderer : public IComponent
{
public:
    virtual ~IModelRenderer() = default;

    virtual void AnimateMatrix(int matrix, float key) = 0;

    virtual void SetModel(TSModelResource* model) = 0;
    virtual TSModelResource* GetModel() = 0;
};