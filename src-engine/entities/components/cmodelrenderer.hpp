#pragma once

#include "entities/components/modelrenderer.hpp"

#include <map>

class CModelRenderer : public IModelRenderer
{
public:
    CModelRenderer();
    ~CModelRenderer() override;

    virtual void Update();

    virtual void AnimateMatrix(int matrix, float key);

    virtual void SetModel(TSModelResource* model);
    virtual TSModelResource* GetModel();
private:

    void AnimateOneMatrix(int matrix, float key);

    TSModelResource* model;

    glm::mat4* animated_matrices;
};
