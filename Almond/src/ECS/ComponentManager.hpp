#pragma once
#include "ECSTypes.h"

class ComponentManager
{
public:
	template<typename T>
	void CreateComponent()
	{
		int componentId = GetId<T>();
		if (componentPools.size() <= componentId) // Not enough component pools
		{
			componentPools.push_back(new ComponentPool(sizeof(T)));
		}

	}

	template<typename T>
	T* AddComponent(Entity entity, T component)
	{
		int componentId = GetId<T>();
		T* pComponent = new (componentPools[componentId]->get(GetEntityIndex(entity))) T();
		*pComponent = component;
		return pComponent;
	}

	template<typename T>
	void RemoveComponent(Entity entity, T component)
	{
		
	}

	template<typename T>
	T* GetComponent(Entity entity, int componentId)
	{
		T* pComponent = static_cast<T*>(componentPools[componentId]->get(GetEntityIndex(entity)));
		return pComponent;
	}

	void EntityDestroyed(Entity entity)
	{
	
	}

private:
	std::vector<ComponentPool*> componentPools;

};




