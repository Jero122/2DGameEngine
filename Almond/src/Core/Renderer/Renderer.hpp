#pragma once
#include <iostream>
#include "Core/ECS/ECSTypes.hpp"
#include "GL/glew.h"
#include "SDL/SDL.h"

class Renderer: public System
{
public:
	explicit Renderer(SDL_Window* window)
		: mWindow(window)
	{

		SDL_GLContext mainContext = SDL_GL_CreateContext(mWindow);
		if (glewInit() != GLEW_OK)
		{
			std::cout << "GLEW DIDNT INIT";
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	}

	void start() override
	{
		
	}
	void update() override
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(mWindow);
		glFlush();
	}


private:
	SDL_Window* mWindow;
};


