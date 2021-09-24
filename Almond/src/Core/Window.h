#pragma once
#include <iostream>
#include <SDL/SDL.h>

#include "Base.h"
#include "Renderer/OldCamera.h"


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

	/*
	Camera camera = Camera();*/
private:
	/*Window()
	{
		std::cout << "Window manager initialised" << std::endl;
		camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}*/
};

