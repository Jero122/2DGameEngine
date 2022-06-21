#pragma once
#include <cstdint>
#include <bitset>
#include <set>
#include <vector>

typedef std::uint32_t EntityID;
//2^20 max entities = 1,048,576 entities
typedef std::uint32_t EntityIndex;
//2^10 max version = 4096
typedef std::uint16_t EntityVersion;
const EntityID MAX_ENTITIES = 120000;

typedef std::uint8_t ComponentType;
constexpr ComponentType MAX_COMPONENTS = 32;

typedef std::bitset <MAX_COMPONENTS>  Signature;
class System
{
public:
	virtual void Init() {};
	virtual void OnUpdate() {};
	virtual void ShutDown() {};
};

inline EntityID CreateEntityId(EntityIndex index, EntityVersion version)
{
	return ((EntityID)index << 12) | ((EntityID)version);
}

inline EntityIndex GetEntityIndex(EntityID entity)
{
	//shift top 32 bits to replace bottom 32 bits:
	//Replaces version with index
	return entity >> 12;
}

inline EntityVersion GetEntityVersion(EntityID entity)
{
	//casting to 32 bits gets rid of top 32 bits
	return (EntityVersion)entity;
}

inline bool isEntityValid(EntityID entity)
{
	// Check if the index is our invalid index
	return (entity >> 12) != EntityIndex(-1);
}

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