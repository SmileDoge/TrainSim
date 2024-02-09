#include "crenderframe.hpp"

#include "modules/crendermodule.hpp"

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

	items[material].emplace_back(mesh, material, transform);
}

void CRenderFrame::AddRenderItem(RenderItem& item)
{
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

void CRenderFrame::Render()
{
	RenderOpaque();
	RenderBlended();
}

void CRenderFrame::RenderMaterial(IMaterial* material, std::vector<RenderItem>& items)
{
	material->PreRender();

	auto view = g_Render->GetCamera()->GetViewMatrix();
	auto proj = g_Render->GetCamera()->GetProjectionMatrix();

	material->Render(items, view, proj);

	material->PostRender();
}

void CRenderFrame::RenderOpaque()
{
	for (auto& [material, items] : items)
	{
		if (material->IsBlended()) continue;

		RenderMaterial(material, items);
	}
}

void CRenderFrame::RenderBlended()
{
	for (auto& [material, items] : items)
	{
		if (!material->IsBlended()) continue;

		RenderMaterial(material, items);
	}
}

void CRenderFrame::Clear()
{
	items.clear();
	lights.clear();
}