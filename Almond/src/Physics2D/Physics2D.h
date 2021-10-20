#pragma once
#include <box2d/b2_world.h>
#include "ECS/ECS.h"

class Physics2D
{
public:
	Physics2D() = default;
	explicit Physics2D(ECS* ecs, b2World* world);

	void SyncWithComponents();
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
	b2World* m_b2World;
};


