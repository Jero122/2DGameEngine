#pragma once
#include "ECS/ECS.hpp"
#include "ECS/ECSTypes.h"

enum BodyType
{
	Static,
	Dynamic,
	Kinematic,
};

class Physics2D
{
public:
	Physics2D() = default;

	explicit Physics2D(ECS* ecs)
		: ecs(ecs)
	{
	}

	void Init();
	void OnUpdate();
	void ShutDown();
	bool interpolate = true; //Interpolates each rigidbody for smoother movement

private:
	float t = 0.0f;
	float dt = 0.01f;

	float m_CurrentTime = 0.0f;
	float m_Accumulator = 0.0f;

	void Interpolate(float alpha);

	ECS* ecs;
};


