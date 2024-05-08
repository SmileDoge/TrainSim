#include "cmodelrenderer.hpp"

#include "modules/crendermodule.hpp"
#include "entities/components/transform.hpp"

#include "modules/cresourcemanager.hpp"

CModelRenderer::CModelRenderer() : model(NULL), animated_matrices(NULL)
{
}

CModelRenderer::~CModelRenderer()
{
    if (model != NULL)
    {
        g_ResourceManager->DecrementRefResource(model);
        delete[] animated_matrices;
    }
}

#include "imgui.h"

void CModelRenderer::Update()
{
    if (model == NULL) return;

    auto camera = g_Render->GetCamera();

    auto& data = model->GetData();

    //auto& matrices = data.matrices;
    auto& matrices = animated_matrices;

    if (!camera->InFOV(GetEntity()->GetTransform()->GetRelativeToCamera(), 100))
        return;

    auto camera_distance = WorldLocation::GetDistanceSquared(g_Render->GetCamera()->GetWorldLocation(), GetEntity()->GetTransform()->GetWorldPosition().WLocation());

    auto& base_matrix = GetEntity()->GetTransform()->GetMatrix();

    
    static std::map<int, bool> disabled_mat;

    ImGui::Begin(GetEntity()->GetName().c_str());

    for (int i = 0; i < data.matrices.size(); i++)
    {
        ImGui::Checkbox(("Hide " + data.matrices_name[i]).c_str(), &disabled_mat[i]);
    }


    ImGui::End();
    

    for (auto& lod : data.lods)
    {
        if (camera_distance > (lod.distance * 0.5f * lod.distance * 0.5f))
            continue;

        for (auto& primitive : lod.primitives)
        {
            auto iNode = primitive.iHierarchy;

            if (disabled_mat[iNode])
                continue;

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

void CModelRenderer::AnimateMatrix(int matrix, float key)
{
    if (model == NULL) return;

    AnimateOneMatrix(matrix, key);

    auto& hierarchy = model->GetData().lods[0].hierarchy;

    for (int i = 0; i < hierarchy.size(); i++)
        if (hierarchy[i] == matrix)
            AnimateMatrix(i, key);
}

#include <algorithm>

void CModelRenderer::AnimateOneMatrix(int matrix, float key)
{
    auto& data = model->GetData();

    if (data.animations.size() == 0) return;

    auto& anim_node = data.animations[0].anim_nodes[matrix];

    if (anim_node.controllers.size() == 0)
        return;

    auto pose = data.matrices[matrix];

    for (auto& controller : anim_node.controllers)
    {
        int index = 0;

        auto& keys = controller.key_positions;

        for (int i = 0; i < keys.size(); i++)
            if (keys[i].frame <= key)
                index = i;
            else if (keys[i].frame > key)
                break;

        auto& position1 = keys[index];
        auto& position2 = index + 1 < keys.size() ? keys[index + 1] : keys[index];

        float frame1 = position1.frame;
        float frame2 = position2.frame;

        auto amount = frame1 < frame2 ? std::clamp((key - frame1) / (frame2 - frame1), 0.f, 1.f) : 0;

        if (position1.type == TS_KEY_POSITION_SLERP_ROT || position2.type == TS_KEY_POSITION_TCB_KEY)
        {
            //glm::quat xna1 = glm::quat(position1.w, position1.x, position1.y, position1.z);
            //glm::quat xna2 = glm::quat(position2.w, position2.x, position2.y, position2.z);
            glm::quat xna1 = glm::quat(-position1.w, position1.x, position1.y, position1.z);
            glm::quat xna2 = glm::quat(-position2.w, position2.x, position2.y, position2.z);
            glm::quat q = glm::slerp(xna1, xna2, amount);

            glm::vec3 loc = glm::vec3(pose[3]);

            pose = glm::mat4_cast(q);
            pose[3] = glm::vec4(loc, 1.0f);
        }
        else if (position1.type == TS_KEY_POSITION_LINEAR_POS)
        {
            glm::vec3 xna1 = glm::vec3(position1.x, position1.y, position1.z);
            glm::vec3 xna2 = glm::vec3(position2.x, position2.y, position2.z);

            glm::vec3 loc = glm::mix(xna1, xna2, amount);
            pose[3] = glm::vec4(loc, 1.0f);
        }
    }

    animated_matrices[matrix] = pose;
}

void CModelRenderer::SetModel(TSModelResource* new_model)
{
    if (new_model == NULL && model != NULL)
        g_ResourceManager->DecrementRefResource(model);

    if (new_model != NULL && model == NULL)
        g_ResourceManager->IncrementRefResource(new_model);

    if (new_model != NULL && model != NULL && model != new_model)
    {
        g_ResourceManager->DecrementRefResource(model);
        g_ResourceManager->IncrementRefResource(new_model);
    }

    if (model != NULL)
        delete[] animated_matrices;

    if (new_model != NULL)
    {
        animated_matrices = new glm::mat4[new_model->GetData().matrices.size()];

        for (int i = 0; i < new_model->GetData().matrices.size(); i++)
            animated_matrices[i] = new_model->GetData().matrices[i];
    }

    model = new_model;
}

TSModelResource* CModelRenderer::GetModel()
{
	return model;
}