#pragma once
#include "ECS/ECSTypes.h"

class Physics2DSystem :  public System
{
public:
	void Init() override;
	void Update() override;
	void ShutDown() override;
	bool interpolate = true; //Interpolates each rigidbody for smoother movement

private:
	float t = 0.0f;
	float dt = 0.01f;

	float m_CurrentTime = 0.0f;
	float m_Accumulator = 0.0f;

	void Interpolate(float alpha);
};
