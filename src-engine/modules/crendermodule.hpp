#pragma once

#include "GLFW/glfw3.h"

#include "modules/rendermodule.hpp"

#include "modules/render/crenderframe.hpp"

#include "modules/render/cwindow.hpp"

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

    virtual ITexture* GetGrayTexture();

    virtual void SetCamera(ICamera* camera);
    virtual ICamera* GetCamera();

    virtual void SetSky(ISky* sky);
    virtual ISky* GetSky();

    virtual int GetTotalVideoMemory();
    virtual int GetAvailableVideoMemory();

    virtual int GetTrianglesDrawnCount();

    virtual IRenderFrame* GetRenderFrame();

    virtual IWindow* GetWindow() { return cWindow; };
    virtual void* GetImGuiContext();

    GLFWwindow* GetGLFWWindow() { return Window; };

    void UpdateSize(int width, int height);

    void InitializeSky();
    
    void AddTriangleCount(int count) { triangles_drawn_count += count; };
private:
    void DetectVersion();

    int ver_major, ver_minor;

    int triangles_drawn_count;

    GLFWwindow* Window;
    CWindow* cWindow;

    ICamera* camera;

    IMaterialManager* material_manager;
    IMeshManager* mesh_manager;
    ITextureManager* texture_manager;

    ISky* sky;

    std::map<std::string, IShader*> shaders;

    CRenderFrame render_frame;

    ITexture* gray_texture;
};

extern CRenderModule* g_Render;