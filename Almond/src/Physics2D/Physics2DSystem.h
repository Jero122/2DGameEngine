#pragma once
#include "ECS/ECSTypes.hpp"

class Physics2DSystem :  public System
{
public:
	void Init() override;
	void Update() override;
	void ShutDown() override;
	void EntityAdded(Entity entity) override;
	void EntityRemoved(Entity entity) override;

private:
	float t = 0.0f;
	float dt = 0.01f;

	float m_CurrentTime = 0.0f;
	float m_Accumulator = 0.0f;

	void Interpolate(float alpha);
};
