#include "cengine.hpp"
#include "modules/crendermodule.hpp"
#include "modules/ccomponentfactory.hpp"
#include <iostream>

#include "register_components.hpp"

CEngine::CEngine() : modules()
{
    std::cout << "CEngine Created!" << std::endl;

    world = new CWorld();

    AddModule("ComponentFactory", new CComponentFactory());

    RegisterComponents();

    AddModule("Render", new CRenderModule());
}

CEngine::~CEngine()
{
    for (auto& [name, module] : modules)
    {
        module->PreDeinit();
        delete module;
    }

    modules.clear();

    delete world;

    std::cout << "CEngine Destroyed!" << std::endl;
}

void CEngine::RunLoop()
{
    while (!glfwWindowShouldClose(g_Render->GetWindow()))
    {
        Update();
    }
}

void CEngine::Update()
{
    g_Render->UpdateRender();

    for (auto& [name, module] : modules)
    {
        module->Update();
    }
}

void CEngine::GetBuildInfo(char*& date, char*& time, long& cppVersion, char*& compiler)
{
    date = ENGINE_BUILDED_DATE;
    time = ENGINE_BUILDED_TIME;
    cppVersion = ENGINE_BUILDED_CPP_VER;
    compiler = ENGINE_COMPILER;
}

void CEngine::GetEngineVersion(short& major, short& minor, short& patch)
{
    major = ENGINE_VERSION_MAJOR;
    minor = ENGINE_VERSION_MINOR;
    patch = ENGINE_VERSION_PATCH;
}

/*

    virtual void GetBuildInfo(char* date, char* time, long& cppVersion, char* compiler);
    virtual void GetEngineVersion(char& major, char& minor, char& patch);
    */

IModule* CEngine::GetModule(std::string name)
{
    if (modules.find(name) == modules.end())
        return NULL;

    return modules[name];
}

void CEngine::AddModule(std::string name, IModule* module)
{
    if (module == NULL) return;

    if (modules.find(name) != modules.end())
        DeleteModule(name);

    modules[name] = module;

    module->PostInit();
}

void CEngine::DeleteModule(std::string name)
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