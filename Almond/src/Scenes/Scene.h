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

	void OnRuntimeStart();
	void OnRuntimeStop();

	//TODO remove editor camera from runtime and use a camera component
	void OnRuntimeUpdate(TimeStep timestep, EditorCamera& editorCamera);
	void OnEditorUpdate(TimeStep timestep, EditorCamera& editorCamera);

private:
	ECS m_Ecs;
	b2World* m_PhysicsWorld;
	Physics2D* m_Physics2D;

	friend class Entity;
	friend class SceneHierarchyPanel;
	friend class SceneSerializer;
};
