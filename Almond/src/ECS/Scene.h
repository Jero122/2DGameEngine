#pragma once
#include "ECS.hpp"
#include "Core/TimeStep.h"
#include "Physics2D/Physics2D.h"

class Entity;

class Scene
{
public:
	Scene();
	~Scene();
	
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	void OnUpdate(TimeStep timestep);
private:
	ECS m_Ecs;
	Physics2D m_Physics2D;
	
	friend class Entity;
};
