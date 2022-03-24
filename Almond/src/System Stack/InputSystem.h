#pragma once
#include "Core/Input.h"
#include "Core/GameSystem.h"

class InputSystem : public GameSystem
{
public:
	InputSystem()
	{
		
	}
	
	~InputSystem() override
	{
		
	}

	void OnUpdate(TimeStep timeStep) override
	{
		Input::GetInstance()->Listen();
	}
	
	void OnLateUpdate() override
	{
		Input::GetInstance()->Reset();
	}
};

