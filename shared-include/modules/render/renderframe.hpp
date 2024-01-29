#pragma once

#include "mesh.hpp"
#include "material.hpp"
#include <vector>
#include "renderitem.hpp"

class IRenderFrame
{
public:
	virtual ~IRenderFrame() = default;

	virtual void AddRenderItem(IMesh* mesh, IMaterial* material, glm::mat4x4& transform) = 0;
	virtual void AddRenderItem(RenderItem& item) = 0;

	virtual void Render() = 0;

	virtual void Clear() = 0;
};