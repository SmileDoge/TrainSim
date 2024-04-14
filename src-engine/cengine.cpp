#include "cengine.hpp"
#include "modules/crendermodule.hpp"
#include "modules/ccomponentfactory.hpp"
#include "modules/clogmodule.hpp"
#include "modules/cpluginmanager.hpp"
#include "modules/cfilesystem.hpp"
#include "modules/cresourcemanager.hpp"
#include "modules/cnetworkmanager.hpp"
#include <iostream>

#include "register_components.hpp"

#include "Windows.h"

#include "game.hpp"

#include "signal.h"
#include "Psapi.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CEngine::CEngine() : modules(), world(NULL), game(NULL)
{
    setlocale(LC_ALL, ".UTF8");

    AddModule("LogModule", new CLogModule());
    AddModule("FileSystem", new CFileSystem());

    engine_config = new CConfig("engine_config.json");
}

#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

void GetModuleNameAndOffset(void* address)
{
    SymInitialize(GetCurrentProcess(), NULL, TRUE);

    IMAGEHLP_MODULE64 moduleInfo = { sizeof(IMAGEHLP_MODULE64) };
    if (SymGetModuleInfo64(GetCurrentProcess(), (DWORD64)address, &moduleInfo))
    {
        // std::cout << "Module Name: " << moduleInfo.ModuleName << std::endl;
        // std::cout << "Base Address: 0x" << std::hex << moduleInfo.BaseOfImage << std::endl;
        // std::cout << "Relative Address: 0x" << std::hex << ((DWORD64)address - moduleInfo.BaseOfImage) << std::endl;
        g_Log->LogError("Exception place: %s+%08X", moduleInfo.ModuleName, ((DWORD64)address - moduleInfo.BaseOfImage));
    }
    else
    {
        g_Log->LogError("Failed to get module information. Error code: %u", GetLastError());
    }

    SymCleanup(GetCurrentProcess());
}

LONG SignalHandler(_EXCEPTION_POINTERS exception)
{
    g_Log->LogError("Unhandled exception!\n\tException code: %08X\n\tException Address: %p", exception.ExceptionRecord->ExceptionCode, exception.ExceptionRecord->ExceptionAddress);
    
    wchar_t buffer[1024];

    swprintf(buffer, L"Unhandled exception! Check Console for more information\n\tException code: %08X\n\tException Address: %p", exception.ExceptionRecord->ExceptionCode, exception.ExceptionRecord->ExceptionAddress);

    GetModuleNameAndOffset(exception.ExceptionRecord->ExceptionAddress);

    MessageBox(NULL, buffer, L"Unhandled exception!", MB_ICONERROR);

    return 0;
}

const char* GetBackendName(RenderBackend backend)
{
    switch (backend)
    {
    case RENDER_BACKEND_OPENGL:
        return "OpenGL";
    default:
        return "Unknown";
    }
}

TSResult CEngine::Initialize()
{
    if (game == NULL)
    {
        g_Log->LogError("game == NULL");
        return TS_INVALID_OPERATION;
    }

    game_config = new CConfig("game_config.json");

    SetFPS(60);

    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)SignalHandler);

    AddModule("PluginManager", new CPluginManager());

    world = new CWorld();

    AddModule("ComponentFactory", new CComponentFactory());
    AddModule("ResourceManager", new CResourceManager());
    AddModule("NetworkManager", new CNetworkManager());

    AddModule("RenderModule", new CRenderModule());

    RegisterComponents();

    IRenderModule* render = GetModule<IRenderModule>();

    ((CRenderModule*)render)->InitializeSky();

    int render_major, render_minor;

    render->GetVersion(render_major, render_minor);

    g_Log->LogDebug("Render Backend: %s", GetBackendName(render->GetBackend()));
    g_Log->LogDebug("Render Version: %d.%d", render_major, render_minor);

    g_Log->LogDebug("Engine Initialized!");

    game->Start();
    game->RegisterComponents();
    game->PostStart();

    g_Log->LogDebug("Game Initialized!");

    /*
    auto curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:7777");

    std::string data;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    CURLcode status = curl_easy_perform(curl);

    long code;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

    g_Log->LogInfo("127.0.0.1:7777 = %d", code);
    g_Log->LogString(ILogType::INFO, data);
    
    curl_easy_cleanup(curl);
    */

    return TS_OK;
}

CEngine::~CEngine()
{
    delete world;

    for (auto& [name, module] : modules)
        module->PreDeinit();

    if (game != NULL)
    {
        game->Shutdown();
        delete game;
    }

    for (auto& [name, module] : modules)
        delete module;

    modules.clear();
}

void CEngine::RunLoop()
{
    //start_engine_time = std::chrono::high_resolution_clock::now();
    //prev_frame_time = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(g_Render->GetGLFWWindow()))
    {
        //auto current_time = std::chrono::high_resolution_clock::now();

        //auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(current_time - prev_frame_time).count() / 1000000.0f;

        current_time = glfwGetTime();

        double delta_time = current_time - prev_frame_time;

        if (delta_time > target_frametime)
        {
            Update();

            this->delta_time = delta_time;

            prev_frame_time = current_time;
        }
    }
}

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

void CEngine::Update()
{
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    game->ProcessInput();

    game->EarlyUpdate();

    for (auto& [name, module] : modules)
        module->Update();

    world->UpdateEntities();
    game->Update();

    g_Render->UpdateRender();

    world->LateUpdateEntities();
    game->LateUpdate();
}

void CEngine::GetBuildInfo(char*& date, char*& time, long& cppVersion, char*& compiler)
{
    date = ENGINE_BUILDED_DATE;
    time = ENGINE_BUILDED_TIME;
    cppVersion = ENGINE_BUILDED_CPP_VER;
    compiler = ENGINE_COMPILER;
}

TSVersion CEngine::GetEngineVersion()
{
    return TSVersion(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH);
}

void CEngine::SetGame(IGame* game)
{
    this->game = game;
}

IGame* CEngine::GetGame()
{
    return game;
}

IModule* CEngine::GetModuleInternal(const std::string& name)
{
    if (modules.find(name) == modules.end())
        return NULL;

    return modules[name];
}

void CEngine::AddModule(const std::string& name, IModule* module)
{
    if (module == NULL) return;

    if (modules.find(name) != modules.end())
        DeleteModule(name);

    modules[name] = module;

    module->PostInit();
}

void CEngine::DeleteModule(const std::string& name)
{
    IModule* module = modules[name];

    module->PreDeinit();

    modules.erase(name);

    delete module;
}

IWorld* CEngine::GetWorld()
{
    return world;
}

IConfig* CEngine::GetEngineConfig()
{
    return engine_config;
}

IConfig* CEngine::GetGameConfig()
{
    return game_config;
}

void CEngine::SetFPS(float fps)
{
    target_frametime = 1 / fps;
}

float CEngine::GetFPS()
{
    return 1 / target_frametime;
}

float CEngine::GetDeltaTime()
{
    return delta_time;
}

double CEngine::GetCurTime()
{
    return current_time;
}

double CEngine::GetSysTime()
{
    return glfwGetTime();
}

int CEngine::GetCurrentThreadID()
{
    return GetCurrentThreadId();
}

