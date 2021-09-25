#pragma once
#include "ECS.h"
#include "Core/TimeStep.h"
#include "Physics2D/Physics2D.h"
#include "Renderer/EditorCamera.h"

class Entity;

class Scene
{
public:
	Scene();
	~Scene();
	
	Entity CreateEntity(std::string name);
	void DestroyEntity(Entity entity);
	void OnUpdate(TimeStep timestep);

	EditorCamera& GetEditorCamera()
	{
		return m_EditorCamera;
	}

private:
	ECS m_Ecs;
	Physics2D m_Physics2D;
	EditorCamera m_EditorCamera;	
	friend class Entity;
	friend class SceneHierarchyPanel;
};
