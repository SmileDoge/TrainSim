#include "register_components.hpp"

#include "modules/crendermodule.hpp" 
#include "modules/ccomponentfactory.hpp"
#include "modules/cresourcemanager.hpp"
#include "modules/cfilesystem.hpp"
#include "modules/render/material.hpp"


#include "entities/components/ctransform.hpp"
#include "entities/components/crendercomponent.hpp"
#include "entities/components/clightcomponent.hpp"
#include "entities/components/cmodelrenderer.hpp"

#include "resources/cmodelresource.hpp"
#include "resources/ctextureresource.hpp"

#include "modules/render/materials/mstsstandard.hpp"

#define DEFINE_SHADER() IShader* shader;

#define REGISTER_SHADER(name, file_name) \
shader = g_Render->CreateShader(name); \
shader->SetVertexShader(g_FileSystem->ReadFileString("data/shaders/"##file_name##".vs").c_str()); \
shader->SetFragmentShader(g_FileSystem->ReadFileString("data/shaders/"##file_name##".fs").c_str()); \
shader->Compile();

DEFINE_CCOMPONENT_FUNCTIONS(Transform);
DEFINE_CCOMPONENT_FUNCTIONS(RenderComponent);
DEFINE_CCOMPONENT_FUNCTIONS(LightComponent);
DEFINE_CCOMPONENT_FUNCTIONS(ModelRenderer)

#include "modules/clogmodule.hpp"

void RegisterShader(const std::string& name, const std::string& path)
{
	std::string vertex_shader_code, fragment_shader_code;

	vertex_shader_code = g_FileSystem->ReadFileString(path + ".vs");
	fragment_shader_code = g_FileSystem->ReadFileString(path + ".fs");

	IShader* shader = g_Render->CreateShader(name);
	shader->SetVertexShader(vertex_shader_code.c_str());
	shader->SetFragmentShader(fragment_shader_code.c_str());
	shader->Compile();
}

void RegisterComponents()
{
	DEFINE_COMPONENT_INFO();
	DEFINE_SHADER();

	REGISTER_COMPONENT_ENGINE(Transform);
	REGISTER_COMPONENT_ENGINE(RenderComponent);
	REGISTER_COMPONENT_ENGINE(LightComponent);
	REGISTER_COMPONENT_ENGINE(ModelRenderer);

	g_Log->LogError("%s", g_FileSystem->ReadFileString("data/shaders/msts_standard_shader.vs").c_str());

	RegisterShader("msts_standard_shader", "data/shaders/msts_standard_shader");

	g_ResourceManager->RegisterResourceFactory(new ModelResourceFactory());
	g_ResourceManager->RegisterResourceFactory(new TextureResourceFactory());

	g_MaterialManager->RegisterMaterialFactory(new MSTSStandardFactory());
}