#pragma once

#include "mesh.hpp"
#include "material.hpp"
#include "light.hpp"
#include "renderitem.hpp"
#include <vector>
#include <map>

class IRenderFrame
{
public:
	virtual ~IRenderFrame() = default;

	virtual void AddRenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4& transform) = 0;
	virtual void AddRenderItem(RenderItem& item) = 0;

	virtual void AddLight(ILight* light) = 0;

	virtual std::map<IMaterial*, std::vector<RenderItem>>& GetItems() = 0;
	virtual std::vector<ILight*>& GetLights() = 0;

	virtual void Render() = 0;

	virtual void Clear() = 0;
};