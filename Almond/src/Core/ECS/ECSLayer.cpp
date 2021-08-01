#include "ECSLayer.h"

#include <random>
#include <stack>

#include "Core/Input.h"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECS.hpp"
#include "Core/Renderer/RenderBatch.h"

ECS ecs;
Entity entity;


ECSLayer::ECSLayer()
{
	ecs.Init();

	ecs.CreateComponent<SpriteRender>();
	ecs.CreateComponent<Transform>();
}


ECSLayer::~ECSLayer()
{
}

void ECSLayer::OnAttach()
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPositionX(0.0f, 1920.0f);
    std::uniform_real_distribution<float> randPositionY(0.0f, 1080.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(0.8f, 1.5f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

  
    for (int i = 0; i < 57000; ++i)
    {
        auto entity = ecs.CreateEntity();
        {
            auto pos = glm::vec3{ 1.0f,0.0f,0.0f };
            auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
            auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
            ecs.AddComponent(entity, Transform{ glm::vec3(randPositionX(generator),randPositionY(generator), 0),rot,scale });
            ecs.AddComponent(entity, SpriteRender{ 10.0f * randScale(generator), 10.0f * randScale(generator), glm::vec4{255,255,255, 1} });
            m_entities.push(entity);
        }
    }


    entity = ecs.CreateEntity();
    {
        ecs.AddComponent(entity, Transform{ glm::vec3(0,0,0), glm::vec3(0,0,0),glm::vec3(1,1,1) });
        ecs.AddComponent(entity, SpriteRender{ 400, 400, glm::vec4{255,255,255, 1} });
        m_entities.push(entity);
       
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

	if (Input::GetInstance()->GetKey(SDL_SCANCODE_A))
	{
        auto& transform = ecs.GetComponent<Transform>(entity);
        transform.position.x--;
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
    }
}

void ECSLayer::OnImGuiRender()
{
}

void ECSLayer::OnLateUpdate()
{
}
