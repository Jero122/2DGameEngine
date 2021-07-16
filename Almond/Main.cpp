#include <Windows.h>
#include <Core/ECS/ECS.hpp>
#include <GL/glew.h>

#include "Core/WindowManager.hpp"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECSTypes.hpp"
#include "Core/Renderer/Renderer.hpp"
#include "Core/ECS/ComponentManager.hpp"


const float SCR_WIDTH = 1920;
const float SCR_HEIGHT = 1080;

ECS ecs;

int main(int argc, char* argv[])
{
    ecs.Init();
    WindowManager window_manager("OUR OpenGL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);

    ecs.CreateComponent<SpriteRender>();
    ecs.CreateComponent<Transform>();

    auto renderer = ecs.CreateSystem<Renderer>();
    {
        Signature signature;
        signature.set(ecs.GetComponentType<SpriteRender>());
        signature.set(ecs.GetComponentType<Transform>());
        ecs.SetSystemSignature<Renderer>(signature);
    }

    renderer->mWindow = window_manager.window;
    renderer->start();

    

    auto entity = ecs.CreateEntity();
    {
        auto pos = glm::vec3{ 0.0f,0.0f,0.0f };
        auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
        auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
        ecs.AddComponent(entity, Transform{ pos,rot,scale });
        ecs.AddComponent(entity, SpriteRender{ glm::vec3{255,255,255} });
    }
	
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
