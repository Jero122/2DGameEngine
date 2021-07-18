#include <random>
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

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-800.0f, 800.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);
	
    for (int i = 0; i < 300; ++i)
    {
        auto entity = ecs.CreateEntity();
        {
            auto pos = glm::vec3{ 1.0f,0.0f,0.0f };
            auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
            auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
            ecs.AddComponent(entity, Transform{ glm::vec3(randPosition(generator),randPosition(generator), 0),rot,scale});
            ecs.AddComponent(entity, SpriteRender{ 50.0f, 50.0f, glm::vec3{255,255,255} });
        }
    }
   
    renderer->start();
	
    bool running = true;
	while (running)
	{
        Uint32 start_time, frame_time;
        start_time = SDL_GetTicks();
        float fps;
		
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
        frame_time = SDL_GetTicks() - start_time;
        fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;
        std::cout << fps << std::endl;


	}
    SDL_Quit();
    return 0;
}
