#include "Physics2D.h"

#include <box2d/b2_polygon_shape.h>

#include "DebugDrawBox2D.h"

#include "ECS/ECS.h"
#include "ECS/SceneView.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/BoxCollider2D.h"


#include "glm/trigonometric.hpp"
#include "glm/vec2.hpp"
#include <SDL/SDL.h>




DebugDrawBox2D* debugDraw;


Physics2D::Physics2D(ECS* ecs, b2World* world)
	: ecs(ecs), m_b2World(world)
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

	//TODO if editor
	//m_b2World->SetDebugDraw(debugDraw);

}

void Physics2D::SyncWithComponents()
{
	for (EntityID ent : SceneView<RigidBody,BoxCollider2D>(*ecs))
	{
		
		RigidBody* rb = ecs->GetComponent<RigidBody>(ent);
		BoxCollider2D* collider = ecs->GetComponent<BoxCollider2D>(ent);
		Transform* transform = ecs->GetComponent<Transform>(ent);


		rb->Body->SetFixedRotation(rb->FixedRotation);
		rb->Body->SetType((b2BodyType)rb->Type);

		b2Fixture* fixture = rb->Body->GetFixtureList();
		b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();


		shape->SetAsBox(collider->Size.x * transform->scale.x, collider->Size.y * transform->scale.y);
		fixture->SetDensity(collider->Density);
		fixture->SetFriction(collider->Friction);
		fixture->SetRestitution(collider->Restitution);
		fixture->SetRestitutionThreshold(collider->RestitutionThreshold);
	}

}


void Physics2D::OnUpdate()
{
	/*float newTime = SDL_GetTicks() / 1000.0f;
	float frameTime = newTime - m_CurrentTime;
	m_CurrentTime = newTime;
	m_Accumulator += frameTime;

	while (m_Accumulator >= dt)
	{
		m_b2World->Step(dt, 6, 2);

		//Copy Current State into previous states
		for (EntityID ent : SceneView<RigidBody>(*ecs))
		{
			RigidBody* rb = ecs->GetComponent<RigidBody>(ent);

			rb->PreviousPosition.x = rb->Body->GetPosition().x;
			rb->PreviousPosition.y = rb->Body->GetPosition().y;
			rb->PreviousAngle = glm::degrees(rb->Body->GetAngle());
		}

		m_Accumulator -= dt;
		t += dt;
	}

	float alpha = m_Accumulator / dt;
	Interpolate(alpha);*/

	m_b2World->Step(dt, 6, 2);

	//Copy Current State into previous states
	for (EntityID ent : SceneView<Transform, RigidBody>(*ecs))
	{
		auto rb = ecs->GetComponent<RigidBody>(ent);
		auto transform = ecs->GetComponent<Transform>(ent);

		transform->position.x = rb->Body->GetPosition().x;
		transform->position.y = rb->Body->GetPosition().y;
		transform->rotation.z = glm::degrees(rb->Body->GetAngle());
	}

	//SyncWithComponents();
	//TODO if editor
	/*m_b2World->DebugDraw();
	debugDraw->Flush();*/
}


void Physics2D::Interpolate(float alpha)
{
	float oneMinusAlpha = 1.0f - alpha;

	for (EntityID ent : SceneView<Transform, RigidBody>(*ecs))
	{
		auto rb = ecs->GetComponent<RigidBody>(ent);
		auto transform = ecs->GetComponent<Transform>(ent);

		transform->position.x = rb->Body->GetPosition().x * alpha + rb->PreviousPosition.x * oneMinusAlpha;
		transform->position.y = rb->Body->GetPosition().y * alpha + rb->PreviousPosition.y * oneMinusAlpha;
		transform->rotation.z = glm::degrees(rb->Body->GetAngle()) * alpha + rb->PreviousAngle * oneMinusAlpha;
	}
}

void Physics2D::ShutDown()
{
}