#pragma once
#include <box2d/b2_world.h>

class PhysicsWorld
{
private:
	static b2World* world;
	PhysicsWorld();

public:

	static b2World* GetInstance();

};
