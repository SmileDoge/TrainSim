#include "cmaterial.hpp"

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

IMaterial* CMaterialManager::CreateMaterial(const std::string& name, const std::string& shader)
{
	if (materials.find(name) != materials.end())
		return materials[name];

	if (factories.find(shader) == factories.end())
		return NULL;

	IMaterialFactory* factory = factories[shader];

	IMaterial* material = factory->CreateMaterial();

	if (material == NULL) return NULL;

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
	return 1;
	//return ++((CMaterial*)material)->ref_count;
}

int CMaterialManager::MaterialReferenceDecrement(IMaterial* material)
{
	return 0;
	/*
	int new_count = --((CMaterial*)material)->ref_count;

	if (new_count <= 0)
		DeleteMaterial(material);

	return new_count;
	*/
}

void CMaterialManager::RegisterMaterialFactory(IMaterialFactory* factory)
{
	if (factory == NULL) return;

	factories[factory->GetSpecificShader()] = factory;
}
