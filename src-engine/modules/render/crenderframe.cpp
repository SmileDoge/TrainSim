#include "crenderframe.hpp"

#include "modules/crendermodule.hpp"

#include <algorithm>

CRenderFrame::CRenderFrame() : items()
{

}

CRenderFrame::~CRenderFrame()
{

}

void CRenderFrame::AddRenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4& transform)
{
	if (mesh == NULL) return;
	if (material == NULL) return;

	if (material->IsBlended())
		items[NULL].emplace_back(mesh, material, transform);
	else
		items[material].emplace_back(mesh, material, transform);
}

void CRenderFrame::AddRenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4& transform, unsigned int sort_index)
{
	if (mesh == NULL) return;
	if (material == NULL) return;

	if (material->IsBlended())
		items[NULL].emplace_back(mesh, material, transform, sort_index);
	else
		items[material].emplace_back(mesh, material, transform, sort_index);
}

void CRenderFrame::AddRenderItem(RenderItem& item)
{
	if (item.Material->IsBlended())
		items[NULL].push_back(item);
	else
		items[item.Material].push_back(item);
}

void CRenderFrame::AddLight(ILight* light)
{
	lights.push_back(light);
}

std::map<IMaterial*, std::vector<RenderItem>>& CRenderFrame::GetItems()
{
	return items;
}

std::vector<ILight*>& CRenderFrame::GetLights()
{
	return lights;
}

glm::vec3 camera_position;

bool CRenderFrame_Items_Sort(RenderItem& x, RenderItem& y)
{
	float xd = glm::distance2(glm::vec3(x.Transform[3]), camera_position);
	float yd = glm::distance2(glm::vec3(y.Transform[3]), camera_position);

	if (abs(yd - xd) >= 0.1)
		return signbit(yd - xd);

	return signbit((float)(x.SortIndex - y.SortIndex));
}

void CRenderFrame::Sort()
{
	camera_position = g_Render->GetCamera()->GetPosition();

	for (auto& [material, items] : items)
	{
		if (material != NULL)
			continue;

		std::sort(items.begin(), items.end(), CRenderFrame_Items_Sort);
	}
}

void CRenderFrame::Render()
{
	Sort();

	RenderOpaque();
	RenderBlended();
}

void CRenderFrame::RenderMaterial(IMaterial* material, std::vector<RenderItem>& items, IMaterial* prev_material, glm::mat4x4& mat_view, glm::mat4x4& mat_proj)
{
	material->PreRender(prev_material);

	material->Render(items, mat_view, mat_proj);

	material->PostRender();
}

void CRenderFrame::RenderOpaque()
{
	auto view = g_Render->GetCamera()->GetViewMatrix();
	auto proj = g_Render->GetCamera()->GetProjectionMatrix();

	for (auto& [material, items] : items)
	{
		auto real_material = items[0].Material;

		if (real_material->IsBlended()) continue;

		RenderMaterial(real_material, items, NULL, view, proj);
	}
}

void CRenderFrame::RenderBlended()
{
	IMaterial* prev_material = NULL;

	auto view = g_Render->GetCamera()->GetViewMatrix();
	auto proj = g_Render->GetCamera()->GetProjectionMatrix();

	for (auto& [material, items] : items)
	{
		if (material != NULL) continue;

		for (auto& item : items)
		{
			auto real_material = item.Material;

			if (prev_material != real_material)
			{
				if (items_blended.size() > 0)
				{
					prev_material->Render(items_blended, view, proj);
					items_blended.clear();
				}
				if (prev_material != NULL)
					prev_material->PostRender();
				real_material->PreRender(prev_material);
				prev_material = real_material;
			}

			items_blended.push_back(item);
		}
		if (items_blended.size() > 0)
		{
			prev_material->Render(items_blended, view, proj);
			items_blended.clear();
		}
		if (prev_material != NULL)
			prev_material->PostRender();
	}
}

void CRenderFrame::Clear()
{
	items.clear();
	lights.clear();
}