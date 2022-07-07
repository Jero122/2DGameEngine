#include "Scene.h"
#include "ECS/Entity.h"
#include "ECS/SceneView.h"
#include <box2d/b2_polygon_shape.h>
#include "ECS/Components/BoxCollider2D.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/ModelRendererComponent.h"
#include "ECS/Components/MovementComponent.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/SpriteRenderer.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/Transform.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Renderer3D.h"


Scene::Scene()
{
	m_Ecs.Init();
	m_Ecs.CreateComponent<TagComponent>();
	m_Ecs.CreateComponent<SpriteRenderer>();
	m_Ecs.CreateComponent<Transform>();
	m_Ecs.CreateComponent<RigidBody>();
	m_Ecs.CreateComponent<BoxCollider2D>();
	m_Ecs.CreateComponent<MovementComponent>();
	m_Ecs.CreateComponent<ModelRendererComponent>();
	m_Ecs.CreateComponent<LightComponent>();

	m_Renderer3D = new Renderer3D();
	m_Renderer2D = new Renderer2D();
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

void Scene::OnRuntimeStart()
{
	m_PhysicsWorld = new b2World({ 0, -10.0f });
	m_PhysicsWorld->SetAllowSleeping(false);
	m_Physics2D = new Physics2D(&m_Ecs, m_PhysicsWorld);

	for (auto entt : SceneView<RigidBody>(m_Ecs))
	{
		auto entity = Entity{ entt, this };
		auto rb = entity.GetComponent<RigidBody>();

		auto transform = entity.GetComponent<Transform>();

		b2BodyDef bodyDef;
		bodyDef.position = { transform->position.x, transform->position.y };
		bodyDef.angle = (glm::radians(transform->rotation.z));
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
			shape.SetAsBox(collider->Size.x * transform->scale.x, collider->Size.y * transform->scale.y);
			b2FixtureDef fixture;
			fixture.shape = &shape;
			fixture.density = collider->Density;
			fixture.friction = collider->Friction;
			fixture.restitution = collider->Restitution;
			fixture.restitutionThreshold = collider->RestitutionThreshold;
			body->CreateFixture(&fixture);
		}
	}
}

void Scene::OnRuntimeStop()
{
	delete m_PhysicsWorld;
	m_PhysicsWorld = nullptr;
	delete m_Physics2D;
	m_Physics2D = nullptr;
}

void Scene::OnRuntimeUpdate(TimeStep timestep, EditorCamera& editorCamera)
{
	for (auto system: m_Ecs)
	{
		system->OnUpdate();
	}

	for (EntityID ent : SceneView<Transform, MovementComponent>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto move = m_Ecs.GetComponent<MovementComponent>(ent);
		transform->position.y -= move->speed * timestep.GetSeconds();
	}

	Render(editorCamera);
	
	//Physcs
	m_Physics2D->OnUpdate();
}

void Scene::OnEditorUpdate(TimeStep timestep, EditorCamera& editorCamera)
{
	Render(editorCamera);
}

void Scene::Render(EditorCamera& editorCamera)
{
	/*for (EntityID ent : SceneView<Transform, SpriteRenderer>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto sprite = m_Ecs.GetComponent<SpriteRenderer>(ent);
		m_Renderer2D->Submit(transform->position, transform->rotation.z, { transform->scale.x, transform->scale.y }, sprite->color, sprite->textureID, sprite->texCoords);
	}
	m_Renderer2D->EndScene();*/

	m_Renderer3D->BeginScene(editorCamera);
	for (EntityID ent : SceneView<Transform, ModelRendererComponent>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto modelRenderer = m_Ecs.GetComponent<ModelRendererComponent>(ent);
		m_Renderer3D->Submit(modelRenderer->model, transform->position, transform->rotation, transform->scale, ent);
	}

	for (EntityID entt : SceneView<Transform, LightComponent>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(entt);
		auto light = m_Ecs.GetComponent<LightComponent>(entt);
		if (light->type == LightComponent::PointLight)
		{
			m_Renderer3D->SubmitPointLight(transform->position, light->ambient, light->diffuse, light->specular, light->constant, light->linear, light->quadratic);
		}
		else if (light->type == LightComponent::SpotLight)
		{
			m_Renderer3D->SubmitSpotLight(transform->position, light->ambient, light->diffuse, light->specular, light->direction, light->innerCutOff, light->outerCutoff);
		}
		else if (light->type == LightComponent::DirectionLight)
		{
			m_Renderer3D->SetDirectionalLight(light->ambient, light->diffuse, light->specular, light->direction);
		}
	}
	m_Renderer3D->EndScene();

}
