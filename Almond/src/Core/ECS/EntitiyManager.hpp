#pragma once


#include <queue>
#include <array>
#include <cassert>
#include <iostream>

#include "ECSTypes.hpp"


class EntityManager
{
public:
	uint32_t mLivingEntityCount = 0;
	
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			mFreeEntities.push(entity);
			mFreeEntitiesSet.insert(mFreeEntities.back());
		}
		std::cout << "\n size if entities:" << sizeof(mFreeEntities) << std::endl;
	}

	Entity CreateEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existance");
		
		//Take an entity from the front of FreeEntities queue
		Entity entity = mFreeEntities.front();
		mFreeEntities.pop();
		mFreeEntitiesSet.erase(entity);
		
		//increment entity counter
		mLivingEntityCount++;
		
		return entity;
	}

	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity our of range");
		
		//reset signature
		mSignatures[entity].reset();

		if (mFreeEntitiesSet.find(entity) == mFreeEntitiesSet.end())
		{
			mFreeEntities.push(entity);
			mFreeEntitiesSet.insert(std::ref(mFreeEntities.back()));
			mLivingEntityCount--;
		}
		/*//add entity back into the freeList
		mFreeEntities.push(entity);
		//decrement entity counter*/
		
	}
	void SetEntitySignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity our of range");
		mSignatures[entity] = signature;
	}

	Signature GetEntitySignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity our of range");
		return mSignatures[entity];
	}

	const std::set<std::reference_wrapper<Entity>>& getFreeSet()
	{
		return mFreeEntitiesSet;
	}
private:
	std::queue<Entity> mFreeEntities;
	std::set<std::reference_wrapper<Entity>> mFreeEntitiesSet;
	
	std::array<Signature, MAX_ENTITIES> mSignatures;

};
