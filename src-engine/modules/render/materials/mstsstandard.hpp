#pragma once

#include "modules/render/material.hpp"

enum MSTSMaterialOptions
{
	MSTS_MATERIAL_NONE = 0,

	MSTS_MATERIAL_DIFFUSE = 0x1,

	MSTS_MATERIAL_ALPHA_TEST = 0x2,

	MSTS_MATERIAL_ALPHA_BLENDING_NONE = 0,
	MSTS_MATERIAL_ALPHA_BLENDING_BLEND = 0x4,
	MSTS_MATERIAL_ALPHA_BLENDING_ADD = 0x8,
	MSTS_MATERIAL_ALPHA_BLENDING_MASK = 0xC,
};

class MSTSStandardMaterial : public IMaterial
{
public:
	MSTSStandardMaterial();
	~MSTSStandardMaterial() override;

	virtual void SetTexture(MaterialStandardParam param, ITexture* val);
	virtual void SetInt(MaterialStandardParam param, int val);

	virtual void PreRender(IMaterial* prev_material);
	virtual void Render(std::vector<RenderItem>& items, glm::mat4x4& mat_view, glm::mat4x4& mat_proj);
	virtual void PostRender();

	virtual bool IsBlended();

	ITexture* albedo_texture;

private:
	int options;

	bool is_one_bit_alpha;
};

DEFINE_MATERIAL_FACTORY(MSTSStandard, "msts_standard_shader")