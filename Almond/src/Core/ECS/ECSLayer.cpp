#include "ECSLayer.h"

#include <random>
#include <stack>

#include "Core/Input.h"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECS.hpp"
#include "Core/Physics2D/Geometry2D.h"
#include "Core/Renderer/RenderBatch.h"
#include "Core/Renderer/SpriteSheet.h"
#include "Core/Renderer/Texture.h"
#include "imgui/imgui.h"
#include "Core/Components/RigidBody.h"
#include "Core/Physics2D/PhysicsWorld.h"

extern ECS ecs;



ECSLayer::ECSLayer()
{
}
std::default_random_engine generator;
std::uniform_real_distribution<float> randPositionX(200.0f, 1800.0f);
std::uniform_real_distribution<float> randPositionY(0.0f, 1000.0f);
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


   
  
    for (int i = 0; i < 200; ++i)
    {
        auto entity = ecs.CreateEntity();
        {
            auto pos = glm::vec3{ randPositionX(generator),randPositionY(generator),0.0f };
            auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
            auto scale = glm::vec3{ 1.0f,1.0f,1.0f };
            ecs.AddComponent(entity, Transform{ glm::vec3(pos.x,pos.y, 0),rot,scale });
            ecs.AddComponent(entity, SpriteRender{ 50.0f , 50.0f, texture1.GetTexID() });

            RigidBody body(*PhysicsWorld::GetInstance(), pos.x, pos.y, BodyType::Dynamic);
            OrientedBox box(25, 25, 1.0f, 0.1f, 0.0f);
            body.AddBoxCollider(box);
        	
            ecs.AddComponent(entity, body);
        }
    }


    Entity ent = ecs.CreateEntity();
    {
        ecs.AddComponent(ent, Transform{ glm::vec3(1000,1080,0), glm::vec3(0,0,0),glm::vec3(1,1,1) });
        ecs.AddComponent(ent, SpriteRender{ 2000, 100, {0,0,0,1} });

        RigidBody body(*PhysicsWorld::GetInstance(), 1000, 1080, BodyType::Static);
        OrientedBox box(1000, 50, 0.0f, 1.0f, 0.0f);

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

	/*if (Input::GetInstance()->GetKey(SDL_SCANCODE_A))
	{
        auto& transform = ecs.GetComponent<Transform>(entity);
	}
    if (Input::GetInstance()->GetKey(SDL_SCANCODE_D))
    {
        auto& transform = ecs.GetComponent<Transform>(entity);
        transform.position.x++;
    }
    if (Input::GetInstance()->GetKey(SDL_SCANCODE_W))
    {
        auto& transform = ecs.GetComponent<Transform>(entity);
        transform.position.y--;
    }
    if (Input::GetInstance()->GetKey(SDL_SCANCODE_S))
    {
        auto& transform = ecs.GetComponent<Transform>(entity);
        transform.position.y++;
    }

    if (Input::GetInstance()->GetKey(SDL_SCANCODE_RIGHT))
    {
        auto& transform = ecs.GetComponent<Transform>(entity);
        transform.scale.x--;
        std::cout << transform.scale.x << std::endl;
    }*/
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
