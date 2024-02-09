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

void CRenderModule_Framebuffer_Resize(GLFWwindow* window, int width, int height)
{
    g_Render->UpdateSize(width, height);
}

CRenderModule::CRenderModule() : renderframe(), shaders(), ver_major(0), ver_minor(0)
{
    g_Render = this;

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DECORATED, FALSE);

    Window = glfwCreateWindow(800, 600, "TrainSimEngine", NULL, NULL);

    cWindow = new CWindow(Window);
    
    glfwSetWindowAttrib(Window, GLFW_RESIZABLE, GLFW_FALSE);

    _ASSERTE(Window != NULL);

    glfwMakeContextCurrent(Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        __debugbreak();
    }

    glfwSetFramebufferSizeCallback(Window, CRenderModule_Framebuffer_Resize);

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_FRONT);

    UpdateSize(800, 600);
}

void CRenderModule::UpdateSize(int width, int height)
{
    if (width == 0 || height == 0) return;

    glViewport(0, 0, width, height);

    camera->SetAspect(((float)width) / ((float)height));
}

void CRenderModule::UpdateRender()
{
    glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderframe.Render();

    renderframe.Clear();

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

    delete camera;

    delete cWindow;

    glfwDestroyWindow(Window);
    
    glfwTerminate();
}
