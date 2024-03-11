#pragma once

#include <string>

#include "glm/vec2.hpp"

enum WindowType
{
	WINDOW_TYPE_WINDOWED,
	WINDOW_TYPE_WINDOWED_FULLSCREEN,
	WINDOW_TYPE_FULLSCREEN
};

class IWindow
{
public:
	virtual ~IWindow() = default;

	virtual void* GetGLFWPointer() = 0;

	virtual void SetSize(glm::vec2 size) = 0;
	virtual glm::vec2 GetSize() = 0;

	virtual void SetPosition(glm::vec2 position) = 0;
	virtual glm::vec2 GetPosition() = 0;

	virtual void SetType(WindowType type) = 0;
	virtual WindowType GetType() = 0;

	virtual bool IsVSyncEnabled() = 0;
	virtual void SetVSync(bool state) = 0;

	// RGBA 32 bit
	virtual void SetIcon(void* pixels, int width, int height) = 0;
	virtual void SetTitle(const std::string& title) = 0;

	virtual void LoadIcon(const std::string& path) = 0;
};