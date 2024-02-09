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

    glm::mat4* matrices_cache;
    bool need_rebuild_cache;
    //std::map<int, glm::mat4> matrices_cache;
};
