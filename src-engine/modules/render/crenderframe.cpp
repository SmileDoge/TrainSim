#include "crenderframe.hpp"

#include "modules/crendermodule.hpp"

#include <algorithm>

CRenderFrame::CRenderFrame() : items(), fullbright(false)
{

}

CRenderFrame::~CRenderFrame()
{

}

bool CRenderFrame::GetFullBright()
{
	return fullbright;
}

void CRenderFrame::SetFullBright(bool state)
{
	fullbright = state;
}

void CRenderFrame::AddRenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4& transform)
{
	AddRenderItem(mesh, material, transform, 0);
}

void CRenderFrame::AddRenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4& transform, unsigned int sort_index)
{
	if (mesh == NULL) return;
	if (material == NULL) return;

	auto is_blended = material->IsBlended();
	
	if (is_blended) // This method of drawing taken from OpenRails https://github.com/openrails/openrails/blob/e0bf062eb64f61f6f41f99d3d49acb8fcc591442/Source/RunActivity/Viewer3D/RenderFrame.cs#L620
	{
		items[NULL].emplace_back(mesh, material, transform, sort_index);
		items[material].emplace_back(mesh, material, transform, sort_index);
	}
	else
	{
		items[material].emplace_back(mesh, material, transform, sort_index);
	}
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

	if (abs(yd - xd) >= 0.001f)
		return (signbit(yd - xd));

	return (signbit((float)(x.SortIndex - y.SortIndex)));
}

void CRenderFrame::Sort()
{
	camera_position = g_Render->GetCamera()->GetLocation();

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

	RenderSky();

	RenderOpaque();
	RenderTransparent();
}

void CRenderFrame::RenderMaterial(IMaterial* material, std::vector<RenderItem>& items, IMaterial* prev_material, glm::mat4x4& mat_view, glm::mat4x4& mat_proj)
{
	material->PreRender(prev_material);

	material->Render(items, mat_view, mat_proj);

	material->PostRender();
}

void CRenderFrame::RenderSky()
{
	if (g_Render->GetSky() == NULL) return;

	g_Render->GetSky()->Render();
}

void CRenderFrame::RenderOpaque()
{
	auto& view = g_Render->GetCamera()->GetViewMatrix();
	auto& proj = g_Render->GetCamera()->GetProjectionMatrix();

	IMaterial* prev_material = NULL;

	for (auto& [material, items] : items)
	{
		if (material == NULL) continue;

		RenderMaterial(material, items, NULL, view, proj);

		prev_material = material;
	}
}

void CRenderFrame::RenderTransparent()
{
	auto& view = g_Render->GetCamera()->GetViewMatrix();
	auto& proj = g_Render->GetCamera()->GetProjectionMatrix();

	for (auto& [material, items] : items)
	{
		if (material != NULL) continue;

		IMaterial* prev_material = NULL;
		for (auto& item : items)
		{
			auto real_material = item.Material;

			if (prev_material != real_material)
			{
				if (items_transparent.size() > 0)
				{
					prev_material->Render(items_transparent, view, proj);
					items_transparent.clear();
				}
				if (prev_material != NULL)
					prev_material->PostRender();
				real_material->PreRender(prev_material);
				prev_material = real_material;
			}

			items_transparent.push_back(item);
		}
		if (items_transparent.size() > 0)
		{
			prev_material->Render(items_transparent, view, proj);
			items_transparent.clear();
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