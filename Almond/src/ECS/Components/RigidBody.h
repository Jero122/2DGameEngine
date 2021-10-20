#pragma once
#include <box2d/b2_body.h>
#include "glm/vec3.hpp"


struct RigidBody
{
	enum class BodyType { Static = 0, Kinematic, Dynamic};
	b2Body* Body;
	BodyType Type = BodyType::Static;
	bool FixedRotation = false;

	glm::vec3 PreviousPosition;
	float PreviousAngle;

	RigidBody() = default;
	RigidBody(const RigidBody&) = default;

};
