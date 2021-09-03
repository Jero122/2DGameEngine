#pragma once
#include "PhysicsWorld.h"

b2World* PhysicsWorld::world = 0;

b2World* PhysicsWorld::GetInstance()
{
	if (world == nullptr)
	{
		world = new b2World({ 0,0 });
	}
	return world;
}

PhysicsWorld::PhysicsWorld()
{}
