#include <Windows.h>
#include <Core/ECS.hpp>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>


#include "Core/ComponentManager.hpp"


const float SCR_WIDTH = 1920;
const float SCR_HEIGHT = 1080;

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	std::cout << "Hello World" << std::endl;

	SDL_Window* window;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(
        "OUR OpenGL Window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        SCR_WIDTH,                               // width, in pixels
        SCR_HEIGHT,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext mainContext = SDL_GL_CreateContext(window);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW DIDNT INIT";
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

	//=====

	struct testComponent
	{
        int id;
	};

    std::unique_ptr<ComponentManager> componentManager = std::make_unique<ComponentManager>();
    componentManager->CreateComponent<testComponent>();
    

    componentManager->AddComponent(0, testComponent{1});


	
    //=====

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

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
        glFlush();
	}
    SDL_Quit();
    return 0;
}
