#include "skymaterial.hpp"

#include "modules/crendermodule.hpp"
#include "cengine.hpp"

#include "glad/glad.h"

#include "modules/render/csky.hpp"

IMaterial* SkyFactory::CreateMaterial()
{
	SkyMaterial* material = new SkyMaterial();

	return material;
}

SkyMaterial::SkyMaterial() : 
	cloud_texture(NULL), 
	moon_map_texture(NULL), 
	moon_mask_texture(NULL), 
	sky_dome_texture(NULL), 
	star_map_n_texture(NULL), 
	star_map_s_texture(NULL),

	sky_shader(NULL),
	moon_shader(NULL),
	clouds_shader(NULL)
{
	night_start = 0.15f;
	night_finish = -0.05f;

	start_color = glm::vec3(0.647f, 0.651f, 0.655f);
	finish_color = glm::vec3(0.05f, 0.05f, 0.05f);

	fog_color = glm::vec3(0, 0, 0);

	sky_shader = g_Render->GetShader("sky_shader");
	moon_shader = g_Render->GetShader("moon_shader");
	clouds_shader = g_Render->GetShader("clouds_shader");
}

SkyMaterial::~SkyMaterial()
{
}

void SkyMaterial::SetTexture(int param, ITexture* val)
{
	SetTexture((SkyMaterialParam)param, val);
}

float Day2Night(float startNightTrans, float finishNightTrans, float minDarknessCoeff, float sunDirectionY)
{
	int vIn = 20;
	float dayAmbientLight = (float)vIn / 20.0f;

	// The following two are used to interpoate between day and night lighting (y = mx + b)
	float slope = (dayAmbientLight - minDarknessCoeff) / (startNightTrans - finishNightTrans); // "m"
	float incpt = dayAmbientLight - slope * startNightTrans; // "b"
	// This is the return value used to darken scenery
	float adjustment;

	if (sunDirectionY < finishNightTrans)
		adjustment = minDarknessCoeff;
	else if (sunDirectionY > startNightTrans)
		adjustment = dayAmbientLight; // Scenery is fully lit during the day
	else
		adjustment = slope * sunDirectionY + incpt;

	return adjustment;
}

void SkyMaterial::Render(RenderItem& item, glm::mat4x4& mat_view, glm::mat4x4& mat_proj)
{
	SkyMesh* mesh = (SkyMesh*)g_Sky->GetSkyMesh();

	mesh->DrawIndex = 1;

	sky_shader->Use();

	sky_shader->SetViewMat(mat_view);
	sky_shader->SetProjMat(mat_proj);

	sky_shader->SetModelMat(item.Transform);

	sky_dome_texture->Use(TEXTURE_UNIT_0);
	star_map_n_texture->Use(TEXTURE_UNIT_1);

	sky_shader->SetInt("starTexture", 1);

	glm::vec3& lightDir = g_Engine->GetWorld()->GetSunDirection();

	auto skyColor1 = Day2Night(0.25f, -0.25f, -0.5f, lightDir.y);
	auto skyColor2 = glm::clamp(skyColor1 + 0.55f, 0.0f, 1.0f);
	auto skyColor3 = 0.001f / (0.8f * glm::abs(lightDir.y - 0.1f));

	sky_shader->SetVec3("SkyColor", glm::vec3(skyColor1, skyColor2, skyColor3));

	sky_shader->SetVec4("lightDirection", glm::vec4(lightDir, 1.0f / glm::length(lightDir)));

	glCullFace(GL_BACK);
	glDepthMask(GL_FALSE);

	item.Mesh->Render();

	glCullFace(GL_FRONT);
}

void SkyMaterial::PostRender()
{
	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
}

void SkyMaterial::SetTexture(SkyMaterialParam param, ITexture* val)
{
	switch (param)
	{
	case MATERIAL_SKY_TEXTURE_CLOUDS:
		cloud_texture = val;
		break;
	case MATERIAL_SKY_TEXTURE_MOON_MAP:
		moon_map_texture = val;
		break;
	case MATERIAL_SKY_TEXTURE_MOON_MASK:
		moon_mask_texture = val;
		break;
	case MATERIAL_SKY_TEXTURE_SKY_DOME:
		sky_dome_texture = val;
		break;
	case MATERIAL_SKY_TEXTURE_STAR_MAP_N:
		star_map_n_texture = val;
		break;
	case MATERIAL_SKY_TEXTURE_STAR_MAP_S:
		star_map_s_texture = val;
		break;
	default:
		break;
	}
}

void SkyMaterial::FogDay2Night(float sun_height, float overcast)
{
	glm::vec3 float_color;

	if (sun_height > night_start)
		float_color = start_color;
	else if (sun_height < night_finish)
		float_color = finish_color;
	else
	{
		auto amount = (sun_height - night_finish) / (night_start - night_finish);
		float_color = glm::mix(finish_color, start_color, amount);
	}

	float_color *= (1 - 0.5f * overcast);

	fog_color.r = float_color.x * 255.0f;
	fog_color.g = float_color.y * 255.0f;
	fog_color.b = float_color.z * 255.0f;
}
