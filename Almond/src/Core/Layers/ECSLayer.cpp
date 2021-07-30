#include "ECSLayer.h"

#include <random>
#include <stack>

#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECS.hpp"

ECS ecs;
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

    std::stack<Entity> entities;

    for (int i = 0; i < 10000; ++i)
    {
        auto entity = ecs.CreateEntity();
        {
            auto pos = glm::vec3{ 1.0f,0.0f,0.0f };
            auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
            auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
            ecs.AddComponent(entity, Transform{ glm::vec3(randPositionX(generator),randPositionY(generator), 0),rot,scale });
            ecs.AddComponent(entity, SpriteRender{ 10.0f * randScale(generator), 10.0f * randScale(generator), glm::vec4{255,255,255, 1} });
            entities.push(entity);
        }
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
}

void ECSLayer::OnImGuiRender()
{
}

void ECSLayer::OnLateUpdate()
{
}
