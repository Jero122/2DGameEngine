#pragma once

#include <memory>
#include "ComponentArray.hpp"

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
		mEntityManger->DestroyEntity(entity);
		mComponentManager->EntityDestroyed(entity);
		mSystemManager->EntityDestroyed(entity);
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
		
		auto signature = mEntityManger->GetEntitySignature();
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManger->SetEntitySignature(entity, signature);
		
		mSystemManager->EntitySignatureChanged(entity, signature);
	}
	template<typename T>
	void RemoveComponent(Entity entity, T component)
	{
		mComponentManager->RemoveComponent(entity, component);
		
		auto signature = mEntityManger->GetEntitySignature();
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
		mSystemManager->CreateSystem<T>();
	}
	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSystemSignature<T>(signature);
	}
	
};
