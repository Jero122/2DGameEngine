#pragma once
#include <iostream>
#include <SDL/SDL.h>

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
private:
	WindowManager()
	{
		std::cout << "This should be called once";
	}
};

