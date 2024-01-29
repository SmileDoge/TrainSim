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

void CRenderFrame::Render()
{
	RenderOpaque();
	RenderBlended();
}

void CRenderFrame::RenderMaterial(IMaterial* material, std::vector<RenderItem>& items)
{
	material->PreRender();

	material->Render(items, g_Render->GetCamera()->GetViewMatrix(), g_Render->GetCamera()->GetProjectionMatrix());

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
}