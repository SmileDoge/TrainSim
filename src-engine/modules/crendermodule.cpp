#include "glad/glad.h"
#include "crendermodule.hpp"

#include <crtdbg.h>
#include <iostream>

CRenderModule* g_Render = NULL;

CRenderModule::CRenderModule()
{
    g_Render = this;

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window = glfwCreateWindow(800, 600, "TrainSimEngine", NULL, NULL);

    _ASSERTE(Window != NULL);

    glfwMakeContextCurrent(Window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        __debugbreak();
    }
}

void CRenderModule::UpdateRender()
{
    glClearColor(0.7f, 0.75f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(Window);
    glfwPollEvents();
}

CRenderModule::~CRenderModule()
{
    glfwDestroyWindow(Window);

    glfwTerminate();
}
