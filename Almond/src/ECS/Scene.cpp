#include "Scene.h"
#include "Entity.h"
#include "SceneView.h"
#include "Components/RigidBody.h"
#include "Components/SpriteRender.h"
#include "Components/Transform.h"
#include "Renderer/Renderer2D.h"

Scene::Scene()
{
	m_Ecs.Init();
	m_Ecs.CreateComponent<SpriteRender>();
	m_Ecs.CreateComponent<Transform>();
	m_Ecs.CreateComponent<RigidBody>();

	m_Physics2D = Physics2D(&m_Ecs);
	m_Physics2D.Init();
}

Scene::~Scene()
{

}

Entity Scene::CreateEntity()
{
	Entity entity = { m_Ecs.CreateEntity(), this };
	return entity;
}

void Scene::DestroyEntity(Entity entity)
{
	m_Ecs.DestroyEntity(entity.GetHandle());
}

void Scene::OnUpdate(TimeStep timestep)
{
	for (auto system: m_Ecs)
	{
		system->OnUpdate();
	}

	//Render
	Renderer2D::BeginScene();
	for (EntityID ent : SceneView<Transform, SpriteRender>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto sprite = m_Ecs.GetComponent<SpriteRender>(ent);

		Renderer2D::Submit(*transform, *sprite);
	}

	Renderer2D::EndScene();
	
	//Physcs
	m_Physics2D.OnUpdate();
}
