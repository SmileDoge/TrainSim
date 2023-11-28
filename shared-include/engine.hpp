#pragma once

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
    #pragma warning Unknown Compiler!
#endif

#define ENGINE_API extern "C"

class IWorld;

class IEngine
{
public:
	virtual ~IEngine() = default;

    virtual void Update() = 0;

    virtual IModule* GetModule(std::string name) = 0;
    virtual void AddModule(std::string name, IModule* module) = 0;
    virtual void DeleteModule(std::string name) = 0;

    virtual IWorld* GetWorld() = 0;

    virtual void GetBuildInfo(char*& date, char*& time, long& cppVersion, char*& compiler) = 0;
    virtual void GetEngineVersion(short& major, short& minor, short& patch) = 0;

    virtual void RunLoop() = 0;
};

ENGINE_API ENGINE_EXPORT IEngine* CreateEngine();
ENGINE_API ENGINE_EXPORT void FreeEngine();