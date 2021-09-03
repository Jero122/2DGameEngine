#include "Physics2DSystem.h"

#include "DebugDrawBox2D.h"
#include "PhysicsWorld.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Transform.h"
#include "ECS/ECS.hpp"
#include "glm/trigonometric.hpp"


extern ECS ecs;
DebugDrawBox2D* debugDraw;

struct PhysicsObject
{
	Entity entity;
	Transform& transform;
	RigidBody& rigidBody;

	PhysicsObject(Entity entity, Transform& transform, RigidBody& sprite)
		: entity(entity),
		transform(transform),
		rigidBody(sprite)
	{
	}

	PhysicsObject(const PhysicsObject& other)
		: entity(other.entity),
		transform(other.transform),
		rigidBody(other.rigidBody)
	{
	}


	PhysicsObject& operator=(const PhysicsObject& other)
	{
		if (this == &other)
			return *this;
		entity = other.entity;
		transform = other.transform;
		rigidBody = other.rigidBody;
		return *this;
	}
};

std::vector<PhysicsObject> physicsObjects;

void Physics2DSystem::Init()
{
	debugDraw = new DebugDrawBox2D();
	debugDraw->Create();
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	/*flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;*/

	debugDraw->SetFlags(flags);
	
	PhysicsWorld::GetInstance()->SetDebugDraw(debugDraw);
	
}

void Physics2DSystem::Update()
{
	PhysicsWorld::GetInstance()->Step(1.0f / 60.0f, 6, 2);

	for (auto& physics_object : physicsObjects)
	{
		physics_object.transform.position.x = physics_object.rigidBody.body->GetPosition().x;
		physics_object.transform.position.y = physics_object.rigidBody.body->GetPosition().y;
		physics_object.transform.rotation.z = glm::degrees(physics_object.rigidBody.body->GetAngle());
	}





	PhysicsWorld::GetInstance()->DebugDraw();
	debugDraw->Flush();
}

void Physics2DSystem::ShutDown()
{
}

void Physics2DSystem::EntityAdded(Entity entity)
{
	physicsObjects.push_back({ entity, ecs.GetComponent<Transform>(entity), ecs.GetComponent<RigidBody>(entity) });
}

void Physics2DSystem::EntityRemoved(Entity entity)
{
	if (ecs.isAlive(entity))
	{
		for (auto iter = physicsObjects.begin(); iter != physicsObjects.end(); ++iter) {
			if (iter->entity == entity) {
				iter = physicsObjects.erase(iter);
				break;
			}
		}
	}
}
