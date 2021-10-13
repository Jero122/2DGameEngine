#include "Scene.h"
#include "ECS/Entity.h"
#include "ECS/SceneView.h"

#include <box2d/b2_polygon_shape.h>
#include "ECS/Components/BoxCollider2D.h"

#include "ECS/Components/RigidBody.h"


#include "ECS/Components/SpriteRenderer.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/Transform.h"
#include "Renderer/Renderer2D.h"

Scene::Scene()
{
	m_Ecs.Init();
	m_Ecs.CreateComponent<TagComponent>();
	m_Ecs.CreateComponent<SpriteRenderer>();
	m_Ecs.CreateComponent<Transform>();
	m_Ecs.CreateComponent<RigidBody>();
	m_Ecs.CreateComponent<BoxCollider2D>();

	m_PhysicsWorld = new b2World({0, -9.81f});
	m_Physics2D = Physics2D(&m_Ecs, m_PhysicsWorld);

	float aspectRatio = 1920.0f / 1080.0f;
	//glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio , 1.0f, -1.0f, -1.0f, 1.0
	m_EditorCamera = EditorCamera(45.0f, aspectRatio, 0.1f, 1000.0f);
}

Scene::~Scene()
{
	delete m_PhysicsWorld;
}

Entity Scene::CreateEntity(std::string name)
{
	Entity entity = { m_Ecs.CreateEntity(), this };
	entity.AddComponent<Transform>({{0,0,0}, {0,0,0},{1.0f,1.0f,1.0f}});
	TagComponent tagComponent = { name };
	if (tagComponent.tag.empty())
	{
		tagComponent.tag = "Entity";
	}
	entity.AddComponent<TagComponent>(tagComponent);
	return entity;
}

void Scene::DestroyEntity(Entity entity)
{
	m_Ecs.DestroyEntity(entity.GetHandle());
}

void Scene::OnStart()
{
	for (auto entt : SceneView<RigidBody>(m_Ecs))
	{
		auto entity = Entity{ entt, this };
		auto rb = entity.GetComponent<RigidBody>();

		auto transform = entity.GetComponent<Transform>();

		b2BodyDef bodyDef;
		bodyDef.position = { transform->position.x, transform->position.y };
		bodyDef.angle = transform->rotation.z;
		switch (rb->Type)
		{
			case RigidBody::BodyType::Static: bodyDef.type = b2_staticBody;  break;
			case RigidBody::BodyType::Dynamic: bodyDef.type = b2_dynamicBody; break;
			case RigidBody::BodyType::Kinematic: bodyDef.type = b2_kinematicBody; break;
		}

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb->FixedRotation);
		rb->Body = body;

		if (entity.HasComponent<BoxCollider2D>())
		{
			auto collider = entity.GetComponent<BoxCollider2D>();
			b2PolygonShape shape;
			shape.SetAsBox(collider->Size.x * transform->scale.x, collider->Size.y * transform->scale.y, {collider->Offset.x, collider->Offset.y}, transform->rotation.z);
			b2FixtureDef fixture;
			fixture.shape = &shape;
			fixture.density = collider->Density;
			fixture.friction = collider->Friction;
			fixture.restitution = collider->Restition;
			fixture.restitutionThreshold = collider->RestitutionThreshold;
			body->CreateFixture(&fixture);
		}
	}
}

void Scene::OnUpdate(TimeStep timestep)
{
	for (auto system: m_Ecs)
	{
		system->OnUpdate();
	}

	//Render
	Renderer2D::BeginScene(m_EditorCamera);
	for (EntityID ent : SceneView<Transform, SpriteRenderer>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto sprite = m_Ecs.GetComponent<SpriteRenderer>(ent);
		Renderer2D::Submit(transform->position,transform->rotation.z,{transform->scale.x, transform->scale.y}, sprite->color,sprite->textureID, sprite->texCoords);
	}

	Renderer2D::EndScene();
	
	//Physcs
	m_Physics2D.OnUpdate();

	m_EditorCamera.OnUpdate(timestep);
}
