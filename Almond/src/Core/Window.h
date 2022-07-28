#pragma once
#include <iostream>
#include "Base.h"


struct WindowProps
{
	std::string Title;
	uint32_t Width;
	uint32_t Height;


	WindowProps(const std::string& title = "Almond Engine", uint32_t width = 1920, uint32_t height = 1080)
		:Title(title), Width(width), Height(height)
	{}
};

class Window
{
public:


	virtual ~Window() = default;
	virtual void OnUpdate() = 0;
	

	static Scope<Window> Create(const WindowProps& props = WindowProps());

	virtual void* GetNativeWindow() const = 0;
	virtual void* GetGLContext() const = 0;

private:
};

