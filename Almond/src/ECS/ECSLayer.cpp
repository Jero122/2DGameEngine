#include "ECSLayer.h"

#include <random>
#include <stack>

#include "Core/Input.h"
#include "Components/SpriteRender.h"
#include "Components/Transform.h"
#include "ECS/ECS.hpp"
#include "Physics2D/Geometry2D.h"
#include "Renderer/RenderBatch.h"
#include "Renderer/SpriteSheet.h"
#include "Renderer/Texture.h"
#include "imgui/imgui.h"
#include "Components/RigidBody.h"
#include "Physics2D/Physics2D.h"
#include "Physics2D/PhysicsWorld.h"

extern ECS ecs;
extern Camera camera;


ECSLayer::ECSLayer()
{
}
std::default_random_engine generator;
std::uniform_real_distribution<float> randPositionX(-8.0f, 8.0f);
std::uniform_real_distribution<float> randPositionY(0.0f, 8.0f);
std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
std::uniform_real_distribution<float> randScale(0.8f, 1.5f);
std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

ECSLayer::~ECSLayer()
{
}

void ECSLayer::OnAttach()
{
   
    Texture texture("resources/textures/container.jpg");
	
    SpriteSheet spriteSheet("resources/textures/UpArrow.png", 32, 32, 1, 1);
	
    Texture texture1("resources/textures/Crate.jpg");


   
  
    for (int i = 0; i < 500; ++i)
    {
        auto entity = ecs.CreateEntity();
        {
            auto pos = glm::vec3{ randPositionX(generator),randPositionY(generator),0.0f };
            auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
            auto scale = glm::vec3{ 1.0f,1.0f,1.0f };
            ecs.AddComponent(entity, Transform{ glm::vec3(pos.x,pos.y, 0),rot,scale });
            ecs.AddComponent(entity, SpriteRender{ 0.5f , 0.5f, texture1.GetTexID() });

            RigidBody body(*PhysicsWorld::GetInstance(), pos.x, pos.y, BodyType::Dynamic);
            OrientedBox box(0.24, 0.24, 1.0f, 0.1f, 0.0f);
            body.AddBoxCollider(box);
        	
            ecs.AddComponent(entity, body);
        }
    }


    Entity ent = ecs.CreateEntity();
    {
        ecs.AddComponent(ent, Transform{ glm::vec3(0,-4.5,0), glm::vec3(0,0,0),glm::vec3(1,1,1) });
        ecs.AddComponent(ent, SpriteRender{ 16, 1, {255,255,255,1} });

        RigidBody body(*PhysicsWorld::GetInstance(), 0, -4.5, BodyType::Static);
        OrientedBox box(8, 0.50, 0.0f, 1.0f, 0.0f);

        body.AddBoxCollider(box);
        ecs.AddComponent(ent, body);
    }


}

void ECSLayer::OnDetach()
{
}

void ECSLayer::OnUpdate()
{
	
	for (auto system : ecs)
	{
		system->Update();
	}

	//TODO timestep
	if (Input::GetInstance()->GetKey(SDL_SCANCODE_A))
	{
        camera.Move(LEFT, 1.0f/ 120.0f);
	}
    if (Input::GetInstance()->GetKey(SDL_SCANCODE_D))
    {
        camera.Move(RIGHT, 1.0f / 120.0f);
    }
    if (Input::GetInstance()->GetKey(SDL_SCANCODE_W))
    {
        camera.Move(UP, 1.0f / 120.0f);
    }
    if (Input::GetInstance()->GetKey(SDL_SCANCODE_S))
    {
        camera.Move(DOWN, 1.0f / 120.0f);
    }

    if (Input::GetInstance()->getWheelY() > 0)
    {
        camera.Zoom(-1.0f);
    }
    if (Input::GetInstance()->getWheelY() < 0)
    {
        camera.Zoom(1.0f);
    }

   
}

void ECSLayer::OnImGuiRender()
{
    ImGui::Begin("ECS");
    {
        ImGui::Text("Living Entities: %d", ecs.getEntityManager()->mLivingEntityCount);

        if (ImGui::Button("Create Entity"))
        {
            auto entity = ecs.CreateEntity();
            {
                auto pos = glm::vec3{ 1.0f,0.0f,0.0f };
                auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
                auto scale = glm::vec3{ 1.0f,1.0f,1.0f };
                ecs.AddComponent(entity, Transform{ glm::vec3(randPositionX(generator),randPositionY(generator), 0),rot,scale });
                ecs.AddComponent(entity, SpriteRender{ 10.0f * randScale(generator), 10.0f * randScale(generator), glm::vec4{255,255,255, 1} });
            }
            m_entities.push(entity);
        }

        if (ImGui::Button("Destroy Entity"))
        {
            int id = m_entities.top();
            m_entities.pop();
            ecs.DestroyEntity(id);
        }
    }
    ImGui::End();
}

void ECSLayer::OnLateUpdate()
{
}
