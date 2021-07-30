#pragma once
#include <cstdint>
#include <bitset>
#include <set>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 100000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset <MAX_COMPONENTS>;

class System
{
public:
	virtual void init(){};
	virtual void Update(){};
	virtual void shutdown() {};
	virtual void entityAdded(Entity entity) {};
	virtual void entityRemoved(Entity entity) {};
	std::set<Entity> mEntities;
};