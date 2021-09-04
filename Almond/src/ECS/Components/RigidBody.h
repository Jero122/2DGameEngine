#pragma once
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>

#include "glm/vec3.hpp"
#include "Physics2D/Geometry2D.h"
#include "Physics2D/Physics2D.h"

struct RigidBody
{

	b2Body* body;
	glm::vec3 previousPosition;
	float previousAngle;

	RigidBody(b2World& world, float x, float y, BodyType type)
	{
		b2BodyDef bodyDef;
		switch (type)
		{
			case Static: bodyDef.type = b2_staticBody;  break;
			case Dynamic: bodyDef.type = b2_dynamicBody; break;
			case Kinematic: bodyDef.type = b2_kinematicBody; break;
		}
		bodyDef.position.Set(x, y);
		body = world.CreateBody(&bodyDef);
	}

	RigidBody() = default;

	void SetContinuousCollision(bool value)
	{
		body->SetBullet(value);
	}

	void SetActive(bool value)
	{
		SetActive(body);
	}

	void AddBoxCollider(OrientedBox box)
	{
		b2PolygonShape shape;
		shape.SetAsBox(box.halfExtentX, box.halfExtentY, { box.offsetX, box.offsetY }, box.angle);
		b2FixtureDef fixture;
		fixture.shape = &shape;		
		fixture.density = box.density;
		fixture.friction = box.friction;
		fixture.restitution = 0.0f;

		body->CreateFixture(&fixture);
	}

	void AddCircleCollider(Circle)
	{
		//TODO Circles
	}
};
