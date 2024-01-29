#include "glad/glad.h"
#include "crendermodule.hpp"

#include <crtdbg.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "modules/clogmodule.hpp"
#include "modules/render/cmaterial.hpp"
#include "modules/render/cmesh.hpp"
#include "modules/render/ctexture.hpp"
#include "modules/render/cshader.hpp"
#include "modules/render/ccamera.hpp"

CRenderModule* g_Render = NULL;

/*
#include <Ultralight/Ultralight.h>

using namespace ultralight;

void InitUltralight(CRenderModule* self)
{
    Config config;

    //config.user_stylesheet = "body { backgroud: white; opacity: 0.5; } h1 { color: white; }";

    Platform::instance().set_config(config);

    Platform::instance().set_font_loader(GetPlatformFontLoader());

    Platform::instance().set_file_system(GetPlatformFileSystem("."));

    Platform::instance().set_logger(GetDefaultLogger("ultralight.log"));
}

int GenTexture()
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    char textureData[500 * 500 * 4];

    memset(textureData, 127, sizeof(textureData));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 500, 500, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureData);

    return texture;
}


class ViewList : public LoadListener
{
    virtual void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
        const String& url)
    {
        g_Log->LogDebug("frame_id: %f, is_main_frame: %f, url: %s", frame_id, is_main_frame, url.utf8().data());
    }
};
*/

CRenderModule::CRenderModule() : renderframe(), shaders(), ver_major(0), ver_minor(0)
{
    g_Render = this;

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window = glfwCreateWindow(1366, 768, "TrainSimEngine", NULL, NULL);

    glfwSetWindowAttrib(Window, GLFW_RESIZABLE, GLFW_FALSE);

    //GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    _ASSERTE(Window != NULL);

    glfwMakeContextCurrent(Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        __debugbreak();
    }

    DetectVersion();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(Window, true);

    ImGui_ImplOpenGL3_Init("#version 330");

    ImFontConfig config;
    config.OversampleH = 1;
    config.OversampleV = 1;
    config.PixelSnapH = true;
    config.MergeMode = false;
    
    io.Fonts->AddFontFromFileTTF("roboto.ttf", 14, &config, io.Fonts->GetGlyphRangesCyrillic());

    io.Fonts->Build();

    camera = new CCamera();

    materialmanager = new CMaterialManager();
    meshmanager = new CMeshManager();
    texturemanager = new CTextureManager();

    auto version = glGetString(GL_VERSION);

    g_Log->LogDebug("OpenGL Version: %s", version);

    /*
    InitUltralight(this);

    renderer = Renderer::Create();

    ViewConfig view_config;
    view_config.is_accelerated = false;
    view_config.is_transparent = true;

    view = renderer->CreateView(500, 500, view_config, nullptr);

    //view->LoadHTML(u8"<h1>бабах дорогой телефончик 😍😍😎😎😋</h1>");
    view->LoadURL("https://www.google.com/");

    ViewList* viewlist = new ViewList();

    view->set_load_listener(viewlist);

    ul_texture = GenTexture();
    */
}

void CRenderModule::UpdateRender()
{
    glClearColor(0.7f, 0.75f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderframe.Render();

    renderframe.Clear();
    /*
    ImGui::Begin("Ultralight");

    ImGui::Image((void*)(intptr_t)ul_texture, ImVec2(500, 500));

    ImGui::End();

    renderer->Update();

    renderer->Render();

    BitmapSurface* surface = (BitmapSurface*)(view->surface());

    if (!surface->dirty_bounds().IsEmpty())
    {
        void* pixels = surface->LockPixels();

        glBindTexture(GL_TEXTURE_2D, ul_texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 500, 500, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

        surface->UnlockPixels();

        surface->ClearDirtyBounds();
    }
    */

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(Window);
}

void* CRenderModule::GetImGuiContext()
{
    return (void*)ImGui::GetCurrentContext();
}

IRenderFrame* CRenderModule::GetRenderFrame()
{
    return &renderframe;
}

void CRenderModule::SetCamera(ICamera* camera)
{
    this->camera = camera;
}

ICamera* CRenderModule::GetCamera()
{
    return camera;
}

IMaterialManager* CRenderModule::GetMaterialManager()
{
    return materialmanager;
}

IMeshManager* CRenderModule::GetMeshManager()
{
    return meshmanager;
}

ITextureManager* CRenderModule::GetTextureManager()
{
    return texturemanager;
}

IShader* CRenderModule::GetShader(const std::string& name)
{
    if (shaders.find(name) == shaders.end())
        return NULL;

    return shaders[name];
}

IShader* CRenderModule::CreateShader(const std::string& name)
{
    if (shaders.find(name) != shaders.end())
        return shaders[name];

    CShader* shader = new CShader();

    shader->SetName(name);

    shaders[name] = shader;

    return shader;
}

RenderBackend CRenderModule::GetBackend()
{
    return RENDER_BACKEND_OPENGL;
}

void CRenderModule::GetVersion(int& major, int& minor)
{
    major = ver_major;
    minor = ver_minor;
}

bool CRenderModule::ExtensionCanLabel()
{
    if (ver_major >= 4 && ver_minor >= 3)
        return true;

    return false;
}

void CRenderModule::DetectVersion()
{
    auto version = glGetString(GL_VERSION);

    sscanf_s((char*)version, "%d.%d", &ver_major, &ver_minor);
}

CRenderModule::~CRenderModule()
{
    delete materialmanager;
    delete meshmanager;
    delete texturemanager;

    glfwDestroyWindow(Window);

    glfwTerminate();
}
