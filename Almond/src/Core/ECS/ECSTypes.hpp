#pragma once
#include <cstdint>
#include <bitset>
#include <set>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 1000000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset <MAX_COMPONENTS>;

class System
{
public:
	virtual void Init(){};
	virtual void Update(){};
	virtual void ShutDown() {};
	virtual void EntityAdded(Entity entity) {};
	virtual void EntityRemoved(Entity entity) {};
	std::set<Entity> mEntities;
};