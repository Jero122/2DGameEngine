#pragma once
#include <cstdint>
#include <bitset>
#include <set>
#include <vector>

using Entity = std::uint64_t;
const Entity MAX_ENTITIES = 50000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;



class System
{
public:
	virtual void Init(){};
	virtual void Update(){};
	virtual void ShutDown() {};
	std::set<Entity> mEntities;
};

typedef std::uint64_t EntityID;
typedef std::bitset <MAX_COMPONENTS>  Signature;

typedef std::uint32_t EntityIndex;
typedef std::uint32_t EntityVersion;

EntityID CreateEntityId(EntityIndex index, EntityVersion version);

EntityIndex GetEntityIndex(EntityID entity);

EntityVersion GetEntityVersion(EntityID entity);

bool isEntityValid(EntityID entity);



struct ComponentPool
{
	ComponentPool(size_t elementsize)
	{
		// We'll allocate enough memory to hold MAX_ENTITIES, each with element size
		elementSize = elementsize;
		pData = new char[elementSize * MAX_ENTITIES];
	}

	~ComponentPool()
	{
		delete[] pData;
	}

	inline void* get(size_t index)
	{
		// looking up the component at the desired index
		return pData + index * elementSize;
	}

	char* pData{ nullptr };
	size_t elementSize{ 0 };
};




extern int s_componentCounter;
template <class T>
int GetId()
{
	static int s_componentId = s_componentCounter++;
	return s_componentId;
}


