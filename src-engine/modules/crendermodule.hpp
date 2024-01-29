#pragma once
#include "modules/rendermodule.hpp"

#include "modules/render/crenderframe.hpp"

#include <map>
#include <string>

class CRenderModule : public IRenderModule
{
public:
    CRenderModule();
    ~CRenderModule() override;

    virtual void PostInit() {};
    virtual void PreDeinit() {};
    virtual void Update() {};

    void UpdateRender();

    virtual RenderBackend GetBackend();
    virtual void GetVersion(int& major, int& minor);

    virtual bool ExtensionCanLabel();

    virtual IMaterialManager* GetMaterialManager();
    virtual IMeshManager* GetMeshManager();
    virtual ITextureManager* GetTextureManager();

    virtual IShader* GetShader(const std::string& name);
    virtual IShader* CreateShader(const std::string& name);

    virtual void SetCamera(ICamera* camera) ;
    virtual ICamera* GetCamera();

    virtual IRenderFrame* GetRenderFrame();

    virtual GLFWwindow* GetWindow() { return Window; };
    virtual void* GetImGuiContext();

private:
    void DetectVersion();

    int ver_major, ver_minor;

    GLFWwindow* Window;
    ICamera* camera;

    IMaterialManager* materialmanager;
    IMeshManager* meshmanager;
    ITextureManager* texturemanager;

    std::map<std::string, IShader*> shaders;

    CRenderFrame renderframe;
};

extern CRenderModule* g_Render;