#pragma once
#include "ECSTypes.h"
#include "ECS.hpp"
#include "Scene.h"

class Entity
{
public:
	Entity() = default;
	Entity(EntityID handle, Scene* scene)
		:entityHandle(handle),m_Scene(scene)
	{
	}
	Entity(const Entity& other) = default;

	template<typename T>
	T* AddComponent(T component)
	{
		return m_Scene->m_Ecs.AddComponent<T>(entityHandle, component);
	}

	template<typename T>
	void RemoveComponent(T component)
	{
		m_Scene->m_Ecs.RemoveComponent<T>(entityHandle, component);
	}

	template<typename T>
	T* GetComponent()
	{
		return m_Scene->m_Ecs.GetComponent<T>(entityHandle);
	}

	~Entity()
	{
		
	}

	EntityID GetHandle()
	{
		return entityHandle;
	}
private:
	EntityID entityHandle;
	Scene* m_Scene;
};
