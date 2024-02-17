#pragma once

#include "modules/render/renderframe.hpp"

class CRenderFrame : public IRenderFrame
{
public:
	CRenderFrame();
	~CRenderFrame() override;

	virtual void AddRenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4& transform);
	virtual void AddRenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4& transform, unsigned int sort_index);
	virtual void AddRenderItem(RenderItem& item);

	virtual void AddLight(ILight* light);

	virtual std::map<IMaterial*, std::vector<RenderItem>>& GetItems();
	virtual std::vector<ILight*>& GetLights();

	virtual void Render();

	virtual void Clear();

private:
	void RenderOpaque();
	void RenderBlended();

	void RenderMaterial(IMaterial* material, std::vector<RenderItem>& items, IMaterial* prev_material, glm::mat4x4& mat_view, glm::mat4x4& mat_proj);

	void Sort();

	std::map<IMaterial*, std::vector<RenderItem>> items;
	std::vector<RenderItem> items_blended;
	std::vector<ILight*> lights;
};