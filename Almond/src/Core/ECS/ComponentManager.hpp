#pragma once

#include "PackedArray.hpp"
#include "ECSTypes.hpp"

#include <unordered_map>
#include <memory>
#include <string>
class ComponentManager
{
private:
	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> mComponentArrays{};

	std::unordered_map<std::string, std::shared_ptr<IComponentArray>> map;
	
	std::unordered_map<const char*, ComponentType> mComponentTypes{};
	ComponentType mNextComponentType = 0;

	template<typename T>
	std::shared_ptr<PackedArray<T>> GetComponentArray()
	{
		const char* componentName = typeid(T).name();
		assert(mComponentTypes.find(componentName) != mComponentTypes.end() && "Component not created before use");
		return std::static_pointer_cast<PackedArray<T>>(mComponentArrays[componentName]);
	}

public:
	template<typename T>
	void CreateComponent()
	{
		const char* componentName = typeid(T).name();
		assert(mComponentTypes.find(componentName) == mComponentTypes.end() && "Creating component type more than once");
		
		
		mComponentArrays.insert(std::make_pair(std::string(componentName), std::make_shared<PackedArray<T>>()));
		
		mComponentTypes.insert({ componentName, mNextComponentType });
		
		
		mNextComponentType++;
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->insertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->removeData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->getData(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* componentName = typeid(T).name();
		assert(mComponentTypes.find(componentName) != mComponentTypes.end() && "Component not created before use");
		return mComponentTypes[componentName];
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}
};




