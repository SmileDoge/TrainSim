#include "cwindow.hpp"

#include "modules/cfilesystem.hpp"
#include "modules/clogmodule.hpp"

#include "stb_image.h"

CWindow::CWindow(GLFWwindow* window) : window(window), vsync_state(false), type(WINDOW_TYPE_WINDOWED)
{

}

CWindow::~CWindow()
{

}

void* CWindow::GetGLFWPointer()
{
	return window;
}

void CWindow::SetSize(glm::vec2 size)
{
	glfwSetWindowSize(window, size.x, size.y);
}

glm::vec2 CWindow::GetSize()
{
	int width, height;

	glfwGetWindowSize(window, &width, &height);

	return glm::vec2(width, height);
}

void CWindow::SetPosition(glm::vec2 position)
{
	glfwSetWindowPos(window, position.x, position.y);
}

glm::vec2 CWindow::GetPosition()
{
	int x, y;

	glfwGetWindowPos(window, &x, &y);

	return glm::vec2(x, y);
}

void CWindow::SetType(WindowType type)
{
	this->type = type;

	auto size = GetSize();

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	g_Log->LogDebug("Current mode: %dx%d (%d hz)", mode->width, mode->height, mode->refreshRate);

	switch (type)
	{
	case WINDOW_TYPE_WINDOWED:
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowMonitor(window, NULL, 100, 100, size.x, size.y, 0);

		break;
	case WINDOW_TYPE_WINDOWED_FULLSCREEN:
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowMonitor(window, NULL, 0, 0, mode->width, mode->height, 0);

		break;
	case WINDOW_TYPE_FULLSCREEN:
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

		break;
	default:
		break;
	}
}

WindowType CWindow::GetType()
{
	return type;
}

bool CWindow::IsVSyncEnabled()
{
	return vsync_state;
}

void CWindow::SetVSync(bool state)
{
	glfwSwapInterval(state ? 1 : 0);

	vsync_state = state;
}

void CWindow::SetIcon(void* pixels, int width, int height)
{
	GLFWimage image;

	image.width = width;
	image.height = height;
	image.pixels = (unsigned char*)pixels;

	glfwSetWindowIcon(window, 1, &image);
}

void CWindow::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(window, title.c_str());
}

void CWindow::LoadIcon(const std::string& path)
{
	size_t len;

	uint8_t* image = g_FileSystem->ReadFile(path, &len);

	int width, height, channels;

	uint8_t* pixels = stbi_load_from_memory(image, len, &width, &height, &channels, 0);

	if (pixels == NULL) return;

	if (channels == 4) SetIcon(pixels, width, height);

	if (channels == 3)
	{
		uint8_t* pixels_rgba = new uint8_t[width * height * 4];

		for (int i = 0; i < width * height; i++)
		{
			pixels_rgba[i * 4 + 0] = pixels[i * 3 + 0];
			pixels_rgba[i * 4 + 1] = pixels[i * 3 + 1];
			pixels_rgba[i * 4 + 2] = pixels[i * 3 + 2];
			pixels_rgba[i * 4 + 3] = 0xFF;
		}

		SetIcon(pixels_rgba, width, height);

		delete[] pixels_rgba;
	}

	stbi_image_free(pixels);

	return;
}