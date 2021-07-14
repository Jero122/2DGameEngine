#include <Windows.h>
#include <Core/ECS/ECS.hpp>
#include <GL/glew.h>

#include "Core/WindowManager.hpp"
#include "Core/ECS/ECSTypes.hpp"
#include "Core/Renderer/Renderer.hpp"
#include "Core/ECS/ComponentManager.hpp"


const float SCR_WIDTH = 1920;
const float SCR_HEIGHT = 1080;

int main(int argc, char* argv[])
{
    WindowManager window_manager("OUR OpenGL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
    Renderer renderer(window_manager.window);

    bool running = true;
	while (running)
	{
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
                return -1;
            }
        }

        renderer.update();

	}
    SDL_Quit();
    return 0;
}
