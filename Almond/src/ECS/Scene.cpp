#include "Scene.h"

#include <iostream>

#include "Entity.h"
#include "SceneView.h"
#include "Components/RigidBody.h"
#include "Components/SpriteRender.h"
#include "Components/TagComponent.h"
#include "Components/Transform.h"
#include "Renderer/Renderer2D.h"

Scene::Scene()
{
	m_Ecs.Init();
	m_Ecs.CreateComponent<SpriteRender>();
	m_Ecs.CreateComponent<Transform>();
	m_Ecs.CreateComponent<RigidBody>();
	m_Ecs.CreateComponent<TagComponent>();

	m_Physics2D = Physics2D(&m_Ecs);
	m_Physics2D.Init();

	float aspectRatio = 1920.0f / 1080.0f;
	//glm::mat4 projection = glm::ortho(-aspectRatio, aspectRatio , 1.0f, -1.0f, -1.0f, 1.0
	m_EditorCamera = EditorCamera(45.0f, aspectRatio, 0.1f, 1000.0f);
}

Scene::~Scene()
{

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

void Scene::OnUpdate(TimeStep timestep)
{
	for (auto system: m_Ecs)
	{
		system->OnUpdate();
	}

	//Render
	Renderer2D::BeginScene(m_EditorCamera);
	for (EntityID ent : SceneView<Transform, SpriteRender>(m_Ecs))
	{
		auto transform = m_Ecs.GetComponent<Transform>(ent);
		auto sprite = m_Ecs.GetComponent<SpriteRender>(ent);
		Renderer2D::Submit(transform->position,transform->rotation.z,{transform->scale.x, transform->scale.y}, sprite->color,sprite->textureID, sprite->texCoords);
	}

	Renderer2D::EndScene();
	
	//Physcs
	m_Physics2D.OnUpdate();

	m_EditorCamera.OnUpdate(timestep);
}