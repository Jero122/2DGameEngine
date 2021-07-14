#pragma once
#include <SDL/SDL.h>

class WindowManager
{
public:
	WindowManager();
	WindowManager(const char* title, int x, int y, int width, int height, Uint32 flags);
	SDL_Window* window;
private:
	
	
};
