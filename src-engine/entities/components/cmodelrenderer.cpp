#include "cmodelrenderer.hpp"

#include "modules/crendermodule.hpp"
#include "entities/components/transform.hpp"

CModelRenderer::CModelRenderer() : model(NULL), matrices_cache(NULL)
{
    need_rebuild_cache = true;
}

CModelRenderer::~CModelRenderer()
{
    delete[] matrices_cache;
}

void CModelRenderer::Update()
{
    if (model == NULL) return;

    auto& base_matrix = GetEntity()->GetTransform()->GetMatrix();

    auto& matrices = model->GetData().matrices;

    auto& lod = model->GetData().lods[0];

    if (matrices_cache == NULL)
        matrices_cache = new glm::mat4[matrices.size()];

    for (auto& primitive : lod.primitives)
    {
        auto iNode = primitive.iHierarchy;
        auto i = iNode;

        auto matrix = glm::mat4(1.0f);

        if (need_rebuild_cache)
        {
            while (iNode != -1)
            {
                matrix = matrices[iNode] * matrix;
                iNode = lod.hierarchy[iNode];
            }

            matrices_cache[i] = matrix;
        }

        g_Render->GetRenderFrame()->AddRenderItem(primitive.mesh, primitive.material, base_matrix * matrices_cache[i], primitive.sort_index);

        /*
        auto iter = matrices_cache.find(iNode2);

        if (iter == matrices_cache.end())
        {
            while (iNode != -1)
            {
                matrix = matrices[iNode] * matrix;
                iNode = lod.hierarchy[iNode];
            }

            matrices_cache[iNode2] = matrix;

            g_Render->GetRenderFrame()->AddRenderItem(primitive.mesh, primitive.material, base_matrix * matrix);
        }
        else
        {
            matrix = base_matrix * (*iter).second;

            g_Render->GetRenderFrame()->AddRenderItem(primitive.mesh, primitive.material, matrix);
        }
        */
    }

    need_rebuild_cache = false;
}

void CModelRenderer::SetModel(TSModelResource* model)
{
    this->model = model;
}

TSModelResource* CModelRenderer::GetModel()
{
	return model;
}
