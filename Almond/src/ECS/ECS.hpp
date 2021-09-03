#pragma once

#include <memory>
#include "PackedArray.hpp"

#include "EntitiyManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

class ECS
{
private:
	std::unique_ptr<EntityManager> mEntityManger;
	std::unique_ptr < ComponentManager> mComponentManager;
	std::unique_ptr < SystemManager> mSystemManager;
	
public:
	void Init()
	{
		mEntityManger = std::make_unique<EntityManager>();
		mComponentManager = std::make_unique<ComponentManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}

	//ENTITY
	Entity CreateEntity()
	{
		return mEntityManger->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		mSystemManager->EntityDestroyed(entity);
		mEntityManger->DestroyEntity(entity);
		mComponentManager->EntityDestroyed(entity);

	}

	bool isAlive(Entity entity)
	{
		auto& set = mEntityManger->getFreeSet();
		if (set.find(entity) != set.end())
		{
			return false;
		}
		return true;
	}

	//COMPONENT
	template<typename T>
	void CreateComponent()
	{
		mComponentManager->CreateComponent<T>();
	}
	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		mComponentManager->AddComponent(entity, component);
		
		auto signature = mEntityManger->GetEntitySignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManger->SetEntitySignature(entity, signature);
		
		mSystemManager->EntitySignatureChanged(entity, signature);
	}
	template<typename T>
	void RemoveComponent(Entity entity, T component)
	{
		mComponentManager->RemoveComponent(entity, component);
		
		auto signature = mEntityManger->GetEntitySignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManger->SetEntitySignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}
	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}
	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	//SYSTEM
	template<typename T>
	std::shared_ptr<T> CreateSystem()
	{
		auto system = mSystemManager->CreateSystem<T>();
		//m_systems.insert(system);
		return system;
	}

	template<typename T>
	std::shared_ptr<T> CreateManualSystem()
	{
		auto system = mSystemManager->CreateManualSystem<T>();
		return system;
	}
	
	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSystemSignature<T>(signature);
	}

	std::unique_ptr<EntityManager>& getEntityManager()
	{
		return mEntityManger;
	}

	std::vector<std::shared_ptr<System>>::iterator begin() { return m_systems.begin(); }
	std::vector<std::shared_ptr<System>>::iterator end() { return m_systems.end(); }

	std::vector<std::shared_ptr<System>>::const_iterator Begin() const { return m_systems.begin(); }
	std::vector<std::shared_ptr<System>>::const_iterator End() const { return m_systems.end(); }

private:
	std::vector<std::shared_ptr<System>> m_systems;
};
