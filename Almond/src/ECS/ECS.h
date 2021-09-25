#pragma once

#include <memory>
#include "ECSTypes.h"
#include "EntitiyManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class ECS
{
private:
	std::unique_ptr<EntityManager> mEntityManger;
	std::unique_ptr < ComponentManager> mComponentManager;
	std::unique_ptr < SystemManager> mSystemManager;
	std::vector<std::shared_ptr<System>> m_systems;
	
public:
	void Init()
	{
		mEntityManger = std::make_unique<EntityManager>();
		mComponentManager = std::make_unique<ComponentManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}

	//ENTITY
	EntityID CreateEntity()
	{
		return mEntityManger->CreateEntity();
	}

	void DestroyEntity(EntityID entity)
	{
		mEntityManger->DestroyEntity(entity);
	}

	//COMPONENT
	template<typename T>
	void CreateComponent()
	{
		mComponentManager->CreateComponent<T>();
	}
	
	template<typename T>
	T* AddComponent(EntityID entity, T component)
	{
		//Ensures we are not accessing a deleted entity
		if (mEntityManger->entities[GetEntityIndex(entity)].id != entity)
		{
			return nullptr;
		}
		
		mEntityManger->SetEntitySignature(entity, GetId<T>());
		return mComponentManager->AddComponent(entity, component);
		
	}
	
	template<typename T>
	void RemoveComponent(EntityID entity, T component)
	{
		//Ensures we are not accessing a deleted entity
		if (mEntityManger->entities[GetEntityIndex(entity)].id != entity)
		{
			return;
		}

		int componentId = GetId<T>();
		mEntityManger->entities[GetEntityIndex(entity)].signature.reset(componentId);
	}
	
	template<typename T>
	T* GetComponent(EntityID entity)
	{
		int componentId = GetId<T>();

		//Ensures we are not accessing a deleted entity
		if (mEntityManger->entities[GetEntityIndex(entity)].id != entity)
		{
			return nullptr;
		}
		
		if (!mEntityManger->entities[GetEntityIndex(entity)].signature.test(componentId))
		{
			return nullptr;
		}
		
		return mComponentManager->GetComponent<T>(entity, componentId);
	}

	template<typename T>
	bool HasComponent(EntityID entity) const
	{
		int componentId = GetId<T>();

		//Ensures we are not accessing a deleted entity
		if (mEntityManger->entities[GetEntityIndex(entity)].id != entity)
		{
			return false;
		}

		if (!mEntityManger->entities[GetEntityIndex(entity)].signature.test(componentId))
		{
			return false;
		}

		return true;
	}
	//SYSTEM
	template<typename T>
	std::shared_ptr<T> CreateSystem()
	{
		auto system = mSystemManager->CreateSystem<T>();
		return system;
	}

	template<typename T>
	std::shared_ptr<T> CreateManualSystem()
	{
		auto system = mSystemManager->CreateManualSystem<T>();
		return system;
	}
	


	std::unique_ptr<EntityManager>& getEntityManager()
	{
		return mEntityManger;
	}

	std::vector<std::shared_ptr<System>>::iterator begin() { return m_systems.begin(); }
	std::vector<std::shared_ptr<System>>::iterator end() { return m_systems.end(); }

	std::vector<std::shared_ptr<System>>::const_iterator Begin() const { return m_systems.begin(); }
	std::vector<std::shared_ptr<System>>::const_iterator End() const { return m_systems.end(); }


};
