#include "cmodelrenderer.hpp"

#include "modules/crendermodule.hpp"
#include "entities/components/transform.hpp"

CModelRenderer::CModelRenderer() : model(NULL)
{
}

CModelRenderer::~CModelRenderer()
{
    /*
    for (int i = 0; i < model->GetData().lods.size(); i++)
    {
        delete[] matrices_cache[i];
    }
    
    delete[] matrices_cache;
    */
}

#include "imgui.h"

void CModelRenderer::Update()
{
    if (model == NULL) return;

    auto camera = g_Render->GetCamera();

    auto& data = model->GetData();

    auto& matrices = data.matrices;

    if (!camera->InFOV(GetEntity()->GetTransform()->GetRelativeToCamera(), 100))
        return;

    auto camera_distance = WorldLocation::GetDistanceSquared(g_Render->GetCamera()->GetWorldLocation(), GetEntity()->GetTransform()->GetWorldPosition().WLocation());

    auto& base_matrix = GetEntity()->GetTransform()->GetMatrix();

    for (auto& lod : data.lods)
    {
        if (camera_distance > (lod.distance * 0.5f * lod.distance * 0.5f))
            continue;

        for (auto& primitive : lod.primitives)
        {
            auto iNode = primitive.iHierarchy;
            auto i = iNode;

            auto matrix = glm::mat4(1.0f);

            while (iNode != -1)
            {
                matrix = matrices[iNode] * matrix;
                iNode = lod.hierarchy[iNode];
            }


            g_Render->GetRenderFrame()->AddRenderItem(primitive.mesh, primitive.material, base_matrix * matrix, primitive.sort_index);
        }

        break;
    }
}

void CModelRenderer::SetModel(TSModelResource* model)
{
    this->model = model;
}

TSModelResource* CModelRenderer::GetModel()
{
	return model;
}
