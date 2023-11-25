#include <iostream>
#include <cassert>
#include "cengine.hpp"

CEngine* g_Engine = NULL;

ENGINE_API ENGINE_EXPORT IEngine* CreateEngine()
{
    if (g_Engine == NULL) g_Engine = new CEngine();

    return (IEngine*)g_Engine;
}

ENGINE_API ENGINE_EXPORT void FreeEngine()
{
    if (g_Engine == NULL) return;

    delete g_Engine;
}