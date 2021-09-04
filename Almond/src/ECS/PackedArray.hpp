#pragma once
#include "cassert"
#include <unordered_map>
#include <sparsepp/spp.h>
#include "ECSTypes.h"

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};


template<typename T>
class PackedArray : public IComponentArray
{
private:
	//std::array<T, MAX_ENTITIES> mComponentArray{};
	std::vector<T> mComponentArray = std::vector<T>(MAX_ENTITIES);

	spp::sparse_hash_map<Entity, size_t> mEntityToIndexMap{MAX_ENTITIES};
	spp::sparse_hash_map<size_t, Entity> mIndexToEntityMap{ MAX_ENTITIES };

	


public:
	int mCurrentEntityIndex = -1;

	PackedArray()
	{
		mComponentArray = std::vector<T>(MAX_ENTITIES);
	}

	PackedArray(size_t size)
	{
		mComponentArray = std::vector<T>(size);
	}
	
	void insertData(Entity entity, T component)
	{
		//assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");
		
		mCurrentEntityIndex++;
		mComponentArray[mCurrentEntityIndex] = component;
		mEntityToIndexMap[entity] = mCurrentEntityIndex;
		mIndexToEntityMap[mCurrentEntityIndex] = entity;
		
	}

	void removeData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");
		auto removedEntityIndex = mEntityToIndexMap[entity];
		Entity currentEntity = mIndexToEntityMap[mCurrentEntityIndex];

		if (mCurrentEntityIndex >= 0)
		{
			mComponentArray[removedEntityIndex] = mComponentArray[mCurrentEntityIndex];

			mEntityToIndexMap[currentEntity] = removedEntityIndex;
			mIndexToEntityMap[removedEntityIndex] = currentEntity;
			mEntityToIndexMap.erase(entity);
			mIndexToEntityMap.erase(mCurrentEntityIndex);
			mCurrentEntityIndex--;
		}
		else
		{
			//TODO LOG ERROR
		}
	}

	T& getData(Entity entity)
	{
		//assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "retrieving non existant component");
		
		return mComponentArray[mEntityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
		{
			removeData(entity);
		}
	}

	void clear()
	{
		mCurrentEntityIndex = -1;
		mEntityToIndexMap.clear();
		mIndexToEntityMap.clear();
	}

	std::vector<T>& getComponentArray()
	{
		return mComponentArray;
	}
};
