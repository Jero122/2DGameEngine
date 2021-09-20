#pragma once
#include "ECS.hpp"
#include "Core/TimeStep.h"
#include "Physics2D/Physics2D.h"
#include "Renderer/Camera.h"

class Entity;

class Scene
{
public:
	Scene();
	~Scene();
	
	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	void OnUpdate(TimeStep timestep);
	Camera& GetCamera()
	{
		return  m_Camera;
	}

private:
	ECS m_Ecs;
	Physics2D m_Physics2D;
	Camera m_Camera;
	
	
	friend class Entity;
};
