#include <Windows.h>
#include <Core/ECS/ECS.hpp>
#include "Core/WindowManager.hpp"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include <GL/glew.h>
#include <SDL/SDL.h>

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader.h"
#include "stb/stb_image.h"


const float SCR_WIDTH = 1920;
const float SCR_HEIGHT = 1080;

ECS ecs;

int main(int argc, char* argv[])
{
    ecs.Init();
    WindowManager window_manager = WindowManager::instance();
    window_manager.Init("OUR OpenGL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);

    ecs.CreateComponent<SpriteRender>();
    ecs.CreateComponent<Transform>();
    auto renderer = ecs.CreateSystem<Renderer>();
    {
        Signature signature;
        signature.set(ecs.GetComponentType<SpriteRender>());
        signature.set(ecs.GetComponentType<Transform>());
        ecs.SetSystemSignature<Renderer>(signature);
    }

    auto entity = ecs.CreateEntity();
    {
        auto pos = glm::vec3{ 1.0f,0.0f,0.0f };
        auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
        auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
        ecs.AddComponent(entity, Transform{ pos,rot,scale });
        ecs.AddComponent(entity, SpriteRender{100.0f, 100.0f, glm::vec3{255,255,255} });
    }
    renderer->start();

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
        renderer->update();
	}
    SDL_Quit();
    return 0;
}
