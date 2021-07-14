#pragma once


#include <queue>
#include <array>
#include <cassert>
#include "ECSTypes.hpp"


class EntityManager
{
public:
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			mFreeEntities.push(entity);
		}
	}

	Entity CreateEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existance");
		
		//Take an entity from the front of FreeEntities queue
		Entity entity = mFreeEntities.front();
		mFreeEntities.pop();
		//increment entity counter
		mLivingEntityCount++;
		
		return entity;
	}

	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity our of range");
		
		//reset signature
		mSignatures[entity].reset();
		//add entity back into the freeList
		mFreeEntities.push(entity);
		//decrement entity counter
		mLivingEntityCount--;
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

private:
	std::queue<Entity> mFreeEntities;
	std::array<Signature, MAX_ENTITIES> mSignatures;
	uint32_t mLivingEntityCount = 0;
};
