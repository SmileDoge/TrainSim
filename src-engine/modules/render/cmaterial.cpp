#include "cmaterial.hpp"

#include "cmesh.hpp"

#include "modules/crendermodule.hpp"

CMaterial::CMaterial() : name("CMaterial"), shader(NULL), albedo_texture(NULL), ref_count(0)
{

}

CMaterial::~CMaterial()
{

}

void CMaterial::SetTexture(MaterialStandartParam param, ITexture* val)
{
	if (param == MATERIAL_ALBEDO_TEXTURE)
		albedo_texture = val;
}

void CMaterial::PreRender()
{

}

void CMaterial::Render(std::vector<RenderItem>& items, glm::mat4x4& mat_view, glm::mat4x4& mat_proj)
{
	shader->Use();
	
	if (albedo_texture != NULL)
		albedo_texture->Use();

	auto lights = g_Render->GetRenderFrame()->GetLights();

	ILight* light = NULL;

	if (lights.size() > 0)
		light = lights[0];

	shader->SetViewMat(mat_view);
	shader->SetProjMat(mat_proj);

	shader->SetVec3("viewPos", g_Render->GetCamera()->GetPosition());

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

	for (auto& item : items)
	{
		//shader->SetMat4x4("model_mat", item.Transform);
		shader->SetModelMat(item.Transform);

		item.Mesh->Render();
	}
}

void CMaterial::PostRender()
{

}

bool CMaterial::IsBlended()
{
	return false;
}

void CMaterial::SetShader(IShader* shader)
{
	this->shader = shader;
}

IShader* CMaterial::GetShader()
{
	return shader;
}

void CMaterial::SetName(const std::string& name)
{
	this->name = name;
}

std::string& CMaterial::GetName()
{
	return name;
}

CMaterialManager* g_MaterialManager = NULL;

CMaterialManager::CMaterialManager() : materials()
{
	g_MaterialManager = this;
}

CMaterialManager::~CMaterialManager()
{
	for (auto& [name, material] : materials)
		delete material;
}

IMaterial* CMaterialManager::CreateMaterial(const std::string& name)
{
	if (materials.find(name) != materials.end())
		return materials[name];

	CMaterial* material = new CMaterial();

	material->SetName(name);

	materials[name] = material;

	return material;
}

IMaterial* CMaterialManager::GetMaterial(const std::string& name)
{
	if (materials.find(name) != materials.end())
		return materials[name];

	return NULL;
}

bool CMaterialManager::IsMaterialCreated(const std::string& name)
{
	if (materials.find(name) == materials.end())
		return false;

	return true;
}

void CMaterialManager::DeleteMaterial(IMaterial* material)
{
	if (materials.find(material->GetName()) == materials.end())
		return;

	materials.erase(material->GetName());

	delete material;
}

int CMaterialManager::MaterialReferenceIncrement(IMaterial* material)
{
	return ++((CMaterial*)material)->ref_count;
}

int CMaterialManager::MaterialReferenceDecrement(IMaterial* material)
{
	int new_count = --((CMaterial*)material)->ref_count;

	if (new_count <= 0)
		DeleteMaterial(material);

	return new_count;
}