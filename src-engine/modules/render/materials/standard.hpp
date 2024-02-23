#pragma once

#include "modules/render/material.hpp"

/*
{
	"shader": "standard_shader",
	"options": {
		"albedo_texture": "../example_texture.ts_tex",
		"transparent": true,
		"alpha_test": true,
		"alpha_test_value": 0.5
	}
}
*/

class StandardMaterial : public IMaterial
{
public:
	StandardMaterial();
	~StandardMaterial() override;

	virtual void SetTexture(MaterialStandardParam param, ITexture* val);

	virtual void PreRender(IMaterial* prev_material);
	virtual void Render(std::vector<RenderItem>& items, glm::mat4x4& mat_view, glm::mat4x4& mat_proj);
	virtual void PostRender();

	virtual bool IsBlended();

	ITexture* albedo_texture;

private:
	int options;
};

DEFINE_MATERIAL_FACTORY(Standard, "standard_shader")