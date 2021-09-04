#include "Physics2DSystem.h"

#include "DebugDrawBox2D.h"
#include "PhysicsWorld.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Transform.h"
#include "ECS/ECS.hpp"
#include "glm/trigonometric.hpp"

#include <SDL/SDL.h>

#include "ECS/SceneView.h"
#include "glm/vec2.hpp"


extern ECS ecs;
DebugDrawBox2D* debugDraw;

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
		for (EntityID ent : SceneView<RigidBody>(ecs))
		{
			RigidBody* rb = ecs.GetComponent<RigidBody>(ent);

			rb->previousPosition.x = rb->body->GetPosition().x;
			rb->previousPosition.y = rb->body->GetPosition().y;
			rb->previousAngle = glm::degrees(rb->body->GetAngle());
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
	
	for (EntityID ent : SceneView<Transform, RigidBody>(ecs))
	{
		auto rb = ecs.GetComponent<RigidBody>(ent);
		auto transform = ecs.GetComponent<Transform>(ent);
		
		transform->position.x = rb->body->GetPosition().x * alpha + rb->previousPosition.x * oneMinusAlpha;
		transform->position.y = rb->body->GetPosition().y * alpha + rb->previousPosition.y * oneMinusAlpha;
		transform->rotation.z = glm::degrees(rb->body->GetAngle()) * alpha + rb->previousAngle* oneMinusAlpha;
	}
}

void Physics2DSystem::ShutDown()
{
}


