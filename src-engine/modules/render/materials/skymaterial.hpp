#pragma once

#include "modules/render/material.hpp"

#include "glm/glm.hpp"

enum SkyMaterialParam
{
	MATERIAL_SKY_TEXTURE_CLOUDS,
	MATERIAL_SKY_TEXTURE_MOON_MAP,
	MATERIAL_SKY_TEXTURE_MOON_MASK,
	MATERIAL_SKY_TEXTURE_SKY_DOME,
	MATERIAL_SKY_TEXTURE_STAR_MAP_N,
	MATERIAL_SKY_TEXTURE_STAR_MAP_S,
};

class SkyMaterial : public IMaterial
{
public:
	SkyMaterial();
	~SkyMaterial() override;

	virtual void SetTexture(int param, ITexture* val);

	virtual void PreRender(IMaterial* prev_material) {};
	virtual void Render(std::vector<RenderItem>& items, glm::mat4x4& mat_view, glm::mat4x4& mat_proj) {};
	virtual void PostRender();

	virtual void Render(RenderItem& item, glm::mat4x4& mat_view, glm::mat4x4& mat_proj);

	virtual bool IsBlended() { return false; };

	void SetTexture(SkyMaterialParam param, ITexture* val);

private:
	void FogDay2Night(float sun_height, float overcast);

	float night_start, night_finish;

	glm::vec3 start_color, finish_color;

	glm::vec3 fog_color;

	ITexture* cloud_texture;
	ITexture* moon_map_texture;
	ITexture* moon_mask_texture;
	ITexture* sky_dome_texture;
	ITexture* star_map_n_texture;
	ITexture* star_map_s_texture;

	IShader* sky_shader;
	IShader* moon_shader;
	IShader* clouds_shader;
};

DEFINE_MATERIAL_FACTORY(Sky, "sky_shader")