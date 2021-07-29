#include <random>
#include <stack>
#include <Core/ECS/ECS.hpp>
#include "Core/WindowManager.hpp"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include <GL/glew.h>
#include <SDL/SDL.h>

#include "Core/Input.h"
#include "Core/Renderer/Renderer2D.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

const float SCR_WIDTH = 1920;
const float SCR_HEIGHT = 1080;

ECS ecs;

int main(int argc, char* argv[])
{
    ecs.Init();
    WindowManager window_manager = WindowManager::instance();
    window_manager.Init("OUR OpenGL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);

    SDL_GLContext gl_context = SDL_GL_CreateContext(WindowManager::instance().window);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW DIDNT INIT";
    }

    glEnable(GL_MULTISAMPLE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	
    ecs.CreateComponent<SpriteRender>();
    ecs.CreateComponent<Transform>();
    auto renderer = ecs.CreateSystem<Renderer2D>();
    {
        Signature signature;
        signature.set(ecs.GetComponentType<SpriteRender>());
        signature.set(ecs.GetComponentType<Transform>());
        ecs.SetSystemSignature<Renderer2D>(signature);
    }

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPositionX(0.0f, 1920.0f);
    std::uniform_real_distribution<float> randPositionY(0.0f, 1080.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(0.8f, 1.5f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);
	
    std::stack<Entity> entities;

    for (int i = 0; i < 10000; ++i)
    {
        auto entity = ecs.CreateEntity();
        {
            auto pos = glm::vec3{ 1.0f,0.0f,0.0f };
            auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
            auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
            ecs.AddComponent(entity, Transform{ glm::vec3(randPositionX(generator),randPositionY(generator), 0),rot,scale});
            ecs.AddComponent(entity, SpriteRender{ 10.0f * randScale(generator), 10.0f * randScale(generator), glm::vec4{255,255,255, 1} });
            entities.push(entity);
        }
    }


    /*
    auto entity1 = ecs.CreateEntity();
    {
        auto pos = glm::vec3{ SCR_WIDTH/2.0f - 60,SCR_HEIGHT/2.0f,0.0f };
        auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
        auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
        ecs.AddComponent(entity1, Transform{ pos,rot,scale });
        ecs.AddComponent(entity1, SpriteRender{ 100.0f, 100.0f , glm::vec4{255,255,255, 1} });
    }
    auto entity2 = ecs.CreateEntity();
    {
        auto pos = glm::vec3{ SCR_WIDTH / 2.0f + 60,SCR_HEIGHT / 2.0f,0.0f };
        auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
        auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
        ecs.AddComponent(entity2, Transform{ pos,rot,scale });
        ecs.AddComponent(entity2, SpriteRender{ 100.0f, 100.0f , glm::vec4{255,255,255, 1} });
    }
    */

   

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(WindowManager::instance().window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");


    renderer->init();

    int i = -1;

    
	
    bool running = true;
    bool destoryed = false;
	while (running)
	{
    
        Input::GetInstance()->Listen();

        if (Input::GetInstance()->GetKeyUp(SDL_SCANCODE_A))
        {
	        std::cout << "A released" << std::endl;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(WindowManager::instance().window);
        ImGui::NewFrame();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            RenderStats stats = renderer->getRenderStats();
            bool batching = renderer->isBatching();
        	
            ImGui::Begin("Renderer Stats");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
            if (ImGui::Checkbox("use Batching", &batching))
            {
                renderer->setBatching(batching);
            }
            ImGui::End();
        	
            ImGui::Begin("ECS");
            ImGui::Text("Living Entities: %d", ecs.getEntityManager()->mLivingEntityCount);


            if (ImGui::Button("Create Entity"))
            {
                auto entity = ecs.CreateEntity();
                {
                    auto pos = glm::vec3{ 1.0f,0.0f,0.0f };
                    auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
                    auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
                    ecs.AddComponent(entity, Transform{ glm::vec3(randPositionX(generator),randPositionY(generator), 0),rot,scale });
                    ecs.AddComponent(entity, SpriteRender{ 10.0f * randScale(generator), 10.0f * randScale(generator), glm::vec4{255,255,255, 1} });
                }
                entities.push(entity);
            }
        	
            if(ImGui::Button("Destroy Entity"))
            {
                int id = entities.top();
                entities.pop();
                ecs.DestroyEntity(id);
            }

        	ImGui::End();
        }
		
        renderer->update();

        Input::GetInstance()->Reset();
       
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(WindowManager::instance().window);

	}

    renderer->shutdown();
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(WindowManager::instance().window);
	SDL_Quit();
	return 0;
}
