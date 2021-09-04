#pragma once
#include <array>
#include <cassert>

#include "ECSTypes.h"




class EntityManager
{
public:
	uint32_t mLivingEntityCount = 0;

	struct EntDesc
	{
		EntityID id;
		Signature signature;
	};
	
	std::vector<EntDesc> entities;
	
	EntityManager()
	{
		
	}

	EntityID CreateEntity()
	{
		if (!freeEntities.empty())
		{
			EntityIndex newIndex = freeEntities.back();
			freeEntities.pop_back();
			EntityID newID = CreateEntityId(newIndex, GetEntityVersion(entities[newIndex].id));
			entities[newIndex].id = newID;
			return entities[newIndex].id;
		}
		
		entities.push_back({ CreateEntityId(entities.size(), 0), Signature() });
		return entities.back().id;
	}

	void DestroyEntity(EntityID entity)
	{
		EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(entity) + 1);
		entities[GetEntityIndex(entity)].id = newID;
		entities[GetEntityIndex(entity)].signature.reset();
		freeEntities.push_back(GetEntityIndex(entity));
	}


	void SetEntitySignature(EntityID entity, int componentID)
	{
		entities[GetEntityIndex(entity)].signature.set(componentID);
	}

	Signature GetEntitySignature(EntityID entity)
	{
		assert(entity < MAX_ENTITIES && "Entity our of range");
		return entities[GetEntityIndex(entity)].signature;
	}


private:
	std::vector<EntityIndex> freeEntities;



};
