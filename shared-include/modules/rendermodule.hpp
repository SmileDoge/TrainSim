#pragma once
#include "module.hpp"
#include "GLFW/glfw3.h"

class IRenderModule : public IModule
{
public:
	virtual ~IRenderModule() = default;

	virtual GLFWwindow* GetWindow() = 0;
};
