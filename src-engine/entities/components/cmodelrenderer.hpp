#pragma once

#include "entities/components/modelrenderer.hpp"

#include <map>

class CModelRenderer : public IModelRenderer
{
public:
    CModelRenderer();
    ~CModelRenderer() override;

    virtual void Update();

    virtual void SetModel(TSModelResource* model);
    virtual TSModelResource* GetModel();
private:
    TSModelResource* model;
};
