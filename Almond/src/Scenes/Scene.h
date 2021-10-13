#pragma once
#include "ECS/ECS.h"
#include "Core/TimeStep.h"
#include "Physics2D/Physics2D.h"
#include "box2d/b2_world.h"
#include "Renderer/EditorCamera.h"

class Entity;

class Scene
{
public:
	Scene();
	~Scene();
	
	Entity CreateEntity(std::string name);
	void DestroyEntity(Entity entity);

	void OnStart();
	void OnUpdate(TimeStep timestep);

	EditorCamera& GetEditorCamera()
	{
		return m_EditorCamera;
	}

private:
	ECS m_Ecs;
	b2World* m_PhysicsWorld;

	Physics2D m_Physics2D;

	EditorCamera m_EditorCamera;

	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
};
