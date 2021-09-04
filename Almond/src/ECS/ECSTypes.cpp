#include "ECSTypes.h"

EntityID CreateEntityId(EntityIndex index, EntityVersion version)
{
	return ((EntityID)index << 32) | ((EntityID)version);
}

EntityIndex GetEntityIndex(EntityID entity)
{
	//shift top 32 bits to replace bottom 32 bits:
	//Replaces version with index
	return entity >> 32;
}

EntityVersion GetEntityVersion(EntityID entity)
{
	//casting to 32 bits gets rid of top 32 bits
	return (EntityVersion)entity;
}

bool isEntityValid(EntityID entity)
{
	// Check if the index is our invalid index
	return (entity >> 32) != EntityIndex(-1);
}
