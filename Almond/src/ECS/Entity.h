#pragma once
#include "ECSTypes.h"
#include "ECS.h"
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
	void RemoveComponent()
	{
		m_Scene->m_Ecs.RemoveComponent<T>(entityHandle);
	}

	template<typename T>
	T* GetComponent()
	{
		return m_Scene->m_Ecs.GetComponent<T>(entityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_Scene->m_Ecs.HasComponent<T>(entityHandle);
	}

	~Entity()
	{
		
	}

	EntityID GetHandle()
	{
		return entityHandle;
	}


	operator uint64_t() const
	{
		return entityHandle;
	}


	bool operator==(const Entity& other)
	{
		return this->entityHandle == other.entityHandle
			&& this->m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity& other)
	{
		return !(*this == other);
	}

	bool IsValid()
	{
		if (entityHandle != (uint64_t)-1 && m_Scene != nullptr)
		{
			return true;
		}
		return false;
	}

private:


	EntityID entityHandle = -1;
	Scene* m_Scene;
};
