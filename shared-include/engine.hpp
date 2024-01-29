#pragma once

#include "global.hpp"
#include "module.hpp"
#include <string>

#if defined(__clang__)
    #define GCC_COMPILER
#elif defined(__GNUC__) || defined(__GNUG__)
    #define GCC_COMPILER
#elif defined(_MSC_VER)
    #define MSVC_COMPILER
#endif

#if defined(MSVC_COMPILER)
    #define ENGINE_EXPORT __declspec(dllexport)
    #define ENGINE_COMPILER "MSVC"
#elif defined(GCC_COMPILER)
    #define ENGINE_EXPORT __attribute__((visibility("default")))
    #define ENGINE_COMPILER "GCC"
#else
    #pragma message("Unknown Compiler!")
#endif

#define ENGINE_API extern "C"

class IWorld;
class IGame;

class IEngine
{
public:
	virtual ~IEngine() = default;

    virtual TSResult Initialize() = 0;

    virtual void SetGame(IGame* game) = 0;
    virtual IGame* GetGame() = 0;

    virtual void Update() = 0;

    virtual IModule* GetModuleInternal(const std::string& name) = 0;
    virtual void AddModule(const std::string& name, IModule* module) = 0;
    virtual void DeleteModule(const std::string& name) = 0;

    virtual IWorld* GetWorld() = 0;

    virtual void GetBuildInfo(char*& date, char*& time, long& cppVersion, char*& compiler) = 0;
    virtual const char* GetEngineVersion() = 0;
    virtual void GetEngineVersion(short& major, short& minor, short& patch) = 0;

    virtual void RunLoop() = 0;

    virtual void SetFPS(float fps) = 0;
    virtual float GetFPS() = 0;

    virtual float GetDeltaTime() = 0;
    virtual double GetCurTime() = 0;

    template<class T>
    T* GetModule();
};

ENGINE_API ENGINE_EXPORT IEngine* CreateEngine();
ENGINE_API ENGINE_EXPORT void FreeEngine();

#include <typeinfo>
template<class T>
T* IEngine::GetModule()
{
    auto name = std::string(typeid(T).name()).substr(6);

    IModule* mod = GetModuleInternal(name);

    if (mod == NULL)
        mod = GetModuleInternal(name.substr(1));

    return (T*)(void*)mod;
}