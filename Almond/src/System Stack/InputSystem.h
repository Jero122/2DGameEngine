#pragma once
#include "Core/Input.h"
#include "Core/SubSystem.h"

class InputSystem : public SubSystem
{
public:
	InputSystem() = default;
	~InputSystem() = default;

	void OnUpdate(TimeStep timeStep) override
	{
		Input::GetInstance()->Listen();
	}
	
	void OnLateUpdate() override
	{
		Input::GetInstance()->Reset();
	}
};

