#include "Physics2DSystem.h"

#include "DebugDrawBox2D.h"
#include "PhysicsWorld.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Transform.h"
#include "ECS/ECS.hpp"
#include "glm/trigonometric.hpp"

#include <SDL/SDL.h>

#include "glm/vec2.hpp"


extern ECS ecs;
DebugDrawBox2D* debugDraw;

struct PhysicsObject
{
	Entity entity;
	Transform& transform;
	RigidBody& rigidBody;

	glm::vec2 previousPosition;
	float previousAngle;

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
	float newTime = SDL_GetTicks() / 1000.0f;
	float frameTime = newTime - m_CurrentTime;
	m_CurrentTime = newTime;
	m_Accumulator += frameTime;

	while (m_Accumulator >= dt)
	{

		
		
		PhysicsWorld::GetInstance()->Step(dt, 6, 2);

		//Copy Current State into previous states
		for (auto& physics_object : physicsObjects)
		{
			physics_object.previousPosition.x = physics_object.rigidBody.body->GetPosition().x;
			physics_object.previousPosition.y = physics_object.rigidBody.body->GetPosition().y;
			physics_object.previousAngle = glm::degrees(physics_object.rigidBody.body->GetAngle());
		}
		
		m_Accumulator -= dt;
		t += dt;
	}

	float alpha = m_Accumulator / dt;

	Interpolate(alpha);
	
	PhysicsWorld::GetInstance()->DebugDraw();
	debugDraw->Flush();
}


void Physics2DSystem::Interpolate(float alpha)
{
	float oneMinusAlpha = 1.0f - alpha;
	
	for (auto& physics_object : physicsObjects)
	{
		physics_object.transform.position.x = physics_object.rigidBody.body->GetPosition().x * alpha + physics_object.previousPosition.x * oneMinusAlpha;
		physics_object.transform.position.y = physics_object.rigidBody.body->GetPosition().y * alpha + physics_object.previousPosition.y * oneMinusAlpha;
		physics_object.transform.rotation.z = glm::degrees(physics_object.rigidBody.body->GetAngle()) * alpha + physics_object.previousAngle* oneMinusAlpha;
	}
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
