#include "WindowManager.hpp"
#include <cstdio>
#include <cassert>
#include <SDL/SDL.h>
#include <GL/glew.h>

void WindowManager::Init(const char* title,int x, int y, int width, int height, Uint32 flags)
{

	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	WindowManager::instance().window = SDL_CreateWindow(title, x, y, width, height, flags);

	if (WindowManager::instance().window == nullptr)
	{
		printf("Could not create window: %s\n", SDL_GetError());
	}



}

