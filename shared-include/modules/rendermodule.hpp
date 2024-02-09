#pragma once
#include "module.hpp"

#include "render/renderframe.hpp"
#include "render/camera.hpp"
#include "render/shader.hpp"
#include "render/material.hpp"
#include "render/mesh.hpp"
#include "render/texture.hpp"
#include "render/window.hpp"

#define GLM_EULER_VEC3(x, y, z) glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z))
#define GLM_EULER(x, y, z) glm::quat(GLM_EULER_VEC3(x, y, z))

enum RenderBackend
{
	RENDER_BACKEND_OPENGL
};

class IRenderModule : public IModule
{
public:
	virtual ~IRenderModule() = default;
	
	virtual RenderBackend GetBackend() = 0;
	virtual void GetVersion(int& major, int& minor) = 0;

	virtual bool ExtensionCanLabel() = 0;

	virtual IMaterialManager* GetMaterialManager() = 0;
	virtual IMeshManager* GetMeshManager() = 0;
	virtual ITextureManager* GetTextureManager() = 0;

	virtual IShader* GetShader(const std::string& name) = 0;
	virtual IShader* CreateShader(const std::string& name) = 0;

	virtual void SetCamera(ICamera* camera) = 0;
	virtual ICamera* GetCamera() = 0;

	virtual IRenderFrame* GetRenderFrame() = 0;

	virtual IWindow* GetWindow() = 0;
	virtual void* GetImGuiContext() = 0;
};
