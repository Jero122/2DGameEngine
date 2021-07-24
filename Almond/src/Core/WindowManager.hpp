#pragma once
#include <iostream>
#include <SDL/SDL.h>

#include "Renderer/Camera.h"

/*class WindowManager
{
public:
	WindowManager();
	WindowManager(const char* title, int x, int y, int width, int height, Uint32 flags);
	static SDL_Window* window;
private:
};*/

class WindowManager
{
public:
	static WindowManager& instance()
	{
		static WindowManager instance;
		return instance;
	}

	void Init(const char* title, int x, int y, int width, int height, Uint32 flags);

	SDL_Window* window;
	Camera camera = Camera();
private:
	WindowManager()
	{
		std::cout << "Window manager initialised" << std::endl;
		camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
};

