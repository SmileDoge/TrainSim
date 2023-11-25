#pragma once

#include "engine.hpp"

class CEngine : public IEngine
{
public:
    CEngine();
    ~CEngine();

    virtual void SomeFunc();
};

extern CEngine* g_Engine;