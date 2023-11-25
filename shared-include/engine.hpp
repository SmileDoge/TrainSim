#pragma once

#if defined(__clang__)
    #define GCC_COMPILER
#elif defined(__GNUC__) || defined(__GNUG__)
    #define GCC_COMPILER
#elif defined(_MSC_VER)
    #define MSVC_COMPILER
#endif

#if defined(MSVC_COMPILER)
    #define ENGINE_EXPORT __declspec(dllexport)
#elif defined(GCC_COMPILER)
    #define ENGINE_EXPORT __attribute__((visibility("default")))
#else
    #pragma warning Unknown Compiler!
#endif

#define ENGINE_API extern "C"

class IEngine
{
public:
    virtual void SomeFunc() = 0;
};

ENGINE_API ENGINE_EXPORT IEngine* CreateEngine();
ENGINE_API ENGINE_EXPORT void FreeEngine();