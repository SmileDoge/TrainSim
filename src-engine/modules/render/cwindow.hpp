#pragma once

#include "modules/render/window.hpp"
#include "GLFW/glfw3.h"

class CWindow : public IWindow
{
public:
	CWindow(GLFWwindow* window);
	~CWindow() override;

	virtual void* GetGLFWPointer();

	virtual void SetSize(glm::vec2 size);
	virtual glm::vec2 GetSize();

	virtual void SetPosition(glm::vec2 position);
	virtual glm::vec2 GetPosition();

	virtual void SetType(WindowType type);
	virtual WindowType GetType();

	virtual void SetIcon(void* pixels, int width, int height);
	virtual void SetTitle(const std::string& title);

	virtual void LoadIcon(const std::string& path);

private:
	GLFWwindow* window;

	WindowType type;
};