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
#include "modules/render/csky.hpp"

CRenderModule* g_Render = NULL;

void CRenderModule_Framebuffer_Resize(GLFWwindow* window, int width, int height)
{
    g_Render->UpdateSize(width, height);
}

CRenderModule::CRenderModule() : render_frame(), shaders(), ver_major(0), ver_minor(0), triangles_drawn_count(0), sky(NULL)
{
    g_Render = this;

	glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DECORATED, FALSE);


    Window = glfwCreateWindow(800, 600, "TrainSimEngine", NULL, NULL);
    
    const char* desc;
    int err = glfwGetError(&desc);

    g_Log->LogInfo("glfwGetError - %s (%d)", desc, err);

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

    material_manager = new CMaterialManager();
    mesh_manager = new CMeshManager();
    texture_manager = new CTextureManager();

    char gray_texture_data[] = {
        0xcc, 0xcc, 0xcc, 0xff
    };

    gray_texture = texture_manager->CreateTexture("gray_texture");
    gray_texture->SetFilter(TEXTURE_FILTER_NEAREST, TEXTURE_FILTER_NEAREST);
    gray_texture->SetWrap(TEXTURE_WRAP_REPEAT, TEXTURE_WRAP_REPEAT);
    gray_texture->SetData(1, 1, TEXTURE_FORMAT_RGBA, gray_texture_data);

    auto version = glGetString(GL_VERSION);
    auto renderer = glGetString(GL_RENDERER);
    int total_vmem = 0;

    glGetIntegerv(0x9048, &total_vmem);

    g_Log->LogDebug("OpenGL Version: %s", version);
    g_Log->LogDebug("OpenGL Renderer: %s", renderer);
    g_Log->LogDebug("Total Video Memory: %d MB", total_vmem / 1024);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

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

    triangles_drawn_count = 0;

    render_frame.Render();

    render_frame.Clear();

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
    return &render_frame;
}

void CRenderModule::SetCamera(ICamera* camera)
{
    this->camera = camera;
}

ICamera* CRenderModule::GetCamera()
{
    return camera;
}

void CRenderModule::SetSky(ISky* sky)
{
    this->sky = sky;
}

ISky* CRenderModule::GetSky()
{
    return sky;
}

void CRenderModule::InitializeSky()
{
    sky = new CSky();
}

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

int CRenderModule::GetTotalVideoMemory()
{
    int memory;

    glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &memory);

    return memory;
}

int CRenderModule::GetAvailableVideoMemory()
{
    int memory;

    glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &memory);

    return memory;
}

int CRenderModule::GetTrianglesDrawnCount()
{
    return triangles_drawn_count;
}

IMaterialManager* CRenderModule::GetMaterialManager()
{
    return material_manager;
}

IMeshManager* CRenderModule::GetMeshManager()
{
    return mesh_manager;
}

ITextureManager* CRenderModule::GetTextureManager()
{
    return texture_manager;
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

ITexture* CRenderModule::GetGrayTexture()
{
    return gray_texture;
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
    delete material_manager;
    delete mesh_manager;
    delete texture_manager;

    delete camera;

    delete cWindow;

    glfwDestroyWindow(Window);
    
    glfwTerminate();
}
