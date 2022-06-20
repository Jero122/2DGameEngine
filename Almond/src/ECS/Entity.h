#pragma once
#include "ECSTypes.h"
#include "ECS.h"
#include "Scenes/Scene.h"

class Entity
{
public:
	Entity() = default;
	Entity(EntityID handle, Scene* scene)
		:m_entityHandle(handle),m_Scene(scene)
	{
	}
	Entity(const Entity& other) = default;

	template<typename T>
	T* AddComponent(T component)
	{
		return m_Scene->m_Ecs.AddComponent<T>(m_entityHandle, component);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_Scene->m_Ecs.RemoveComponent<T>(m_entityHandle);
	}

	template<typename T>
	T* GetComponent()
	{
		return m_Scene->m_Ecs.GetComponent<T>(m_entityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_Scene->m_Ecs.HasComponent<T>(m_entityHandle);
	}

	~Entity()
	{
		
	}

	EntityID GetHandle()
	{
		return m_entityHandle;
	}


	operator uint64_t() const
	{
		return m_entityHandle;
	}


	bool operator==(const Entity& other)
	{
		return this->m_entityHandle == other.m_entityHandle
			&& this->m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity& other)
	{
		return !(*this == other);
	}

	operator bool() const
	{
		return m_entityHandle != (uint32_t)-1 && m_Scene != nullptr;
	}

	bool IsValid()
	{
		if (m_entityHandle != (uint32_t)-1 && m_Scene != nullptr)
		{
			return true;
		}
		return false;
	}

private:


	EntityID m_entityHandle = -1;
	Scene* m_Scene;
};
