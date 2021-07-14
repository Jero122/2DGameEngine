#pragma once
#include "ECSTypes.hpp"
#include "cassert"
#include <unordered_map>
#include <array>

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};


template<typename T>
class ComponentArray : public IComponentArray
{
private:
	std::array<T, MAX_ENTITIES> mComponentArray{};
	std::unordered_map<Entity, size_t> mEntityToIndexMap{};
	std::unordered_map<size_t, Entity> mIndexToEntityMap{};
	
	int mCurrentEntityIndex = -1;
public:
	void insertData(Entity entity, T component)
	{
		assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");
		
		mCurrentEntityIndex++;
		mComponentArray[mCurrentEntityIndex] = component;
		mEntityToIndexMap[entity] = mCurrentEntityIndex;
		mIndexToEntityMap[mCurrentEntityIndex] = entity;
		
	}

	void removeData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");
		int removedEntityIndex = mEntityToIndexMap[entity];
		Entity currentEntity = mIndexToEntityMap[mCurrentEntityIndex];
		
		mComponentArray[removedEntityIndex] = mComponentArray[mCurrentEntityIndex];

		mEntityToIndexMap[currentEntity] = removedEntityIndex;
		mIndexToEntityMap[removedEntityIndex] = currentEntity;
		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(mCurrentEntityIndex);
		mCurrentEntityIndex--;
	}

	T& getData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "retrieving non existant component");
		
		return mComponentArray[mEntityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
		{
			removeData(entity);
		}
	}
};
