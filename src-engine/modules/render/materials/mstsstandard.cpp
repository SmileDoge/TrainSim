#include "mstsstandard.hpp"

#include "modules/crendermodule.hpp"

#include "glad/glad.h"

MSTSStandardMaterial::MSTSStandardMaterial() : albedo_texture(NULL)
{
}

MSTSStandardMaterial::~MSTSStandardMaterial()
{
}

void MSTSStandardMaterial::SetTexture(MaterialStandardParam param, ITexture* val)
{
	if (param == MATERIAL_ALBEDO_TEXTURE)
		albedo_texture = val;

	if (albedo_texture != NULL)
	{
		TexturePixelFormat format = albedo_texture->GetFormat();
	
		switch (format)
		{
		case TEXTURE_FORMAT_RGBA:
		case TEXTURE_FORMAT_RGB:
		case TEXTURE_FORMAT_BGRA:
		case TEXTURE_FORMAT_BGR:
		case TEXTURE_FORMAT_BGRA4444:
		case TEXTURE_FORMAT_RED:
		case TEXTURE_FORMAT_RGB565:
		case TEXTURE_FORMAT_BGR565:
		case TEXTURE_FORMAT_DXT3:
		case TEXTURE_FORMAT_DXT5:
			is_one_bit_alpha = false;
			break;

		case TEXTURE_FORMAT_RGBA5551:
		case TEXTURE_FORMAT_BGRA5551:
		case TEXTURE_FORMAT_DXT1:
			is_one_bit_alpha = true;
			break;
		default:
			is_one_bit_alpha = false;
			break;
		}
	}
}

void MSTSStandardMaterial::SetInt(MaterialStandardParam param, int val)
{
	if (param == MATERIAL_MSTS_OPTIONS)
		options = val;
}

void MSTSStandardMaterial::PreRender(IMaterial* prev_material)
{
	shader->Use();

	float alpha_test = 0.f;

	glEnable(GL_BLEND);

	if (IsBlended())
	{
		if (prev_material == NULL && (options & MSTS_MATERIAL_ALPHA_BLENDING_MASK) != MSTS_MATERIAL_ALPHA_BLENDING_ADD)
		{
			alpha_test = 250.f / 255.f;

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

			glDepthFunc(GL_LEQUAL);
			glDepthMask(GL_TRUE);
		}
		else
		{
			alpha_test = 10.f / 255.f;

			int sourceBlend = GL_SRC_ALPHA;

			int destinationBlend = GL_ONE;
			int depthFunction = GL_LEQUAL;

			
			if ((options & MSTS_MATERIAL_ALPHA_BLENDING_MASK) == MSTS_MATERIAL_ALPHA_BLENDING_BLEND)
			{
				destinationBlend = GL_ONE_MINUS_SRC_ALPHA;
				depthFunction = GL_LESS;
			}

			glBlendFuncSeparate(GL_SRC_ALPHA, destinationBlend, GL_ZERO, GL_ONE);

			glDepthFunc(depthFunction);
			glDepthMask(GL_FALSE);
		}
	}
	else
	{
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		if ((options & MSTS_MATERIAL_ALPHA_TEST) != 0)
		{
			alpha_test = 200.f / 255.f;
		}
		else
		{
			alpha_test = 0.f;
		}
	}

	shader->SetFloat("alpha_test", alpha_test);
}

void MSTSStandardMaterial::Render(std::vector<RenderItem>& items, glm::mat4x4& mat_view, glm::mat4x4& mat_proj)
{
	if (albedo_texture != NULL)
		albedo_texture->Use();

	auto& lights = g_Render->GetRenderFrame()->GetLights();

	ILight* light = NULL;

	if (lights.size() > 0)
		light = lights[0];

	shader->SetViewMat(mat_view);
	shader->SetProjMat(mat_proj);

	shader->SetVec3("viewPos", g_Render->GetCamera()->GetLocation());

	shader->SetVec3("ambient", glm::vec3(0.4f, 0.4f, 0.4f));
	shader->SetVec3("specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader->SetFloat("shininess", 32.0f);

	if (light)
	{
		shader->SetVec3("light.position", light->GetPosition());
		shader->SetVec3("light.ambient", light->GetAmbient());
		shader->SetVec3("light.diffuse", light->GetDiffuse());
		shader->SetVec3("light.specular", light->GetSpecular());
	}
	else
	{
		shader->SetVec3("light.position", glm::vec3(0.0f));
		shader->SetVec3("light.ambient", glm::vec3(0.1f));
		shader->SetVec3("light.diffuse", glm::vec3(0.1f));
		shader->SetVec3("light.specular", glm::vec3(1.0f));
	}

	shader->SetInt("fullbright", g_Render->GetRenderFrame()->GetFullBright());

	for (auto& item : items)
	{
		shader->SetModelMat(item.Transform);

		item.Mesh->Render();
	}
}

void MSTSStandardMaterial::PostRender()
{
	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	shader->SetFloat("alpha_test", 0.0f);
}

bool MSTSStandardMaterial::IsBlended()
{
	bool alphaTestRequested = (options & MSTS_MATERIAL_ALPHA_TEST) != 0;
	bool alphaBlendRequested = (options & MSTS_MATERIAL_ALPHA_BLENDING_MASK) != 0;

	return alphaBlendRequested && (!is_one_bit_alpha || (is_one_bit_alpha && !alphaTestRequested));
}


IMaterial* MSTSStandardFactory::CreateMaterial()
{
	MSTSStandardMaterial* material = new MSTSStandardMaterial();

	material->SetShader(g_Render->GetShader("msts_standard_shader"));

	return material;
}
