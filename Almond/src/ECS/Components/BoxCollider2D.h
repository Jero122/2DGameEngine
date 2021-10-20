#pragma once
#include <box2d/b2_fixture.h>
#include <glm/vec2.hpp>

struct BoxCollider2D
{
	glm::vec2 Offset = { 0.0f, 0.0f };
	glm::vec2 Size = { 0.0f, 0.0f };

	float Density = 1.0f;
	float Friction = 1.0f;
	float Restition = 0.0f;
	float RestitutionThreshold = 0.5f;

	b2Fixture* Fixture = nullptr;

	BoxCollider2D() = default;
	BoxCollider2D(const BoxCollider2D&) = default;
};
