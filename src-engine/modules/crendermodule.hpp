#pragma once
#include "modules/rendermodule.hpp"

class CRenderModule : public IRenderModule
{
public:
    CRenderModule();
    ~CRenderModule() override;

    virtual void PostInit() {};
    virtual void PreDeinit() {};
    virtual void Update() {};

    void UpdateRender();

    virtual GLFWwindow* GetWindow() { return Window; };

private:

    GLFWwindow* Window;
};

extern CRenderModule* g_Render;