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
#include "box2d/box2d.h"

ECS ecs;
Entity entity;

std::unique_ptr<b2World> world;
b2Body* ent1Body;

bool running = false;


ECSLayer::ECSLayer()
{
	ecs.Init();

	ecs.CreateComponent<SpriteRender>();
	ecs.CreateComponent<Transform>();
}
std::default_random_engine generator;
std::uniform_real_distribution<float> randPositionX(0.0f, 1920.0f);
std::uniform_real_distribution<float> randPositionY(0.0f, 1080.0f);
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

    world = std::make_unique<b2World>(b2Vec2(0, 10));
  
    /*for (int i = 0; i < 10; ++i)
    {
        auto entity = ecs.CreateEntity();
        {
            auto pos = glm::vec3{ 1.0f,0.0f,0.0f };
            auto rot = glm::vec3{ 0.0f,0.0f,0.0f };
            auto scale = glm::vec3{ 0.0f,0.0f,0.0f };
            ecs.AddComponent(entity, Transform{ glm::vec3(randPositionX(generator),randPositionY(generator), 0),rot,scale });
            ecs.AddComponent(entity, SpriteRender{ 100.0f * randScale(generator), 100.0f * randScale(generator), glm::vec4{255,255,255, 1}, texture.GetTexID() });
            m_entities.push(entity);
        }
    }*/

    Line2D line({ 0,0 }, { 10,10 });
    float mag = Length(line);
    float magSq = LengthSquared(line);
	
	
    Entity ent0 = ecs.CreateEntity();
    {
        ecs.AddComponent(ent0, Transform{ glm::vec3(800,800,0), glm::vec3(0,0,0),glm::vec3(1,1,1) });
        ecs.AddComponent(ent0, SpriteRender(400, 400, texture.GetTexID()));

       
    }

    Entity ent1 = ecs.CreateEntity();
    {
        ecs.AddComponent(ent1, Transform{ glm::vec3(500,100,0), glm::vec3(0,0,0),glm::vec3(1,1,1) });
        ecs.AddComponent(ent1, SpriteRender{ 400, 400, texture1.GetTexID() });
      

    }

    Entity ent2 = ecs.CreateEntity();
    {
        ecs.AddComponent(ent2, Transform{ glm::vec3(500,1080,0), glm::vec3(0,0,0),glm::vec3(1,1,1) });
        ecs.AddComponent(ent2, SpriteRender{ 1000, 100, texture1.GetTexID() });


    }

    b2BodyDef ent0BodyDef;
    ent0BodyDef.position.Set(800, 800);
    b2Body* ent0Body = world->CreateBody(&ent0BodyDef);

    b2PolygonShape ent0Box;
    ent0Box.SetAsBox(200, 200);

    ent0Body->CreateFixture(&ent0Box, 0.0f);


    b2BodyDef ent1BodyDef;
    ent1BodyDef.type = b2_dynamicBody;
    ent1BodyDef.position.Set(500, 100);
    ent1Body = world->CreateBody(&ent1BodyDef);

    b2PolygonShape ent1BodyBox;
    ent1BodyBox.SetAsBox(200, 200);
    b2FixtureDef ent1FixtureDef;
    ent1FixtureDef.shape = &ent1BodyBox;
    ent1FixtureDef.density = 1.0f;
    ent1FixtureDef.friction = 0.3f;

    ent1Body->CreateFixture(&ent1FixtureDef);



    b2BodyDef ent2BodyDef;
    ent2BodyDef.position.Set(500, 1080);
    b2Body* ent2Body = world->CreateBody(&ent2BodyDef);

    b2PolygonShape ent2Box;
    ent2Box.SetAsBox(500, 50);

    ent2Body->CreateFixture(&ent2Box, 0.0f);
}

void ECSLayer::OnDetach()
{
}

void ECSLayer::OnUpdate()
{
	if (running)
	{
        world->Step(1.0f / 180.0f, 6, 2);
        auto& transform = ecs.GetComponent<Transform>(1);
        transform.position.x = ent1Body->GetPosition().x;
        transform.position.y = ent1Body->GetPosition().y;
        transform.rotation.z = glm::degrees(ent1Body->GetAngle());
	}
  
	
	for (auto system : ecs)
	{
		system->Update();
	}

	if (Input::GetInstance()->GetKey(SDL_SCANCODE_A))
	{
        auto& transform = ecs.GetComponent<Transform>(entity);
        running = true;
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
