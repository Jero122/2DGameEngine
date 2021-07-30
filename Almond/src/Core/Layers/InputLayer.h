#pragma once
#include "Core/Input.h"
#include "Core/Layer.h"

class InputLayer : public Layer
{
public:
	InputLayer()
	{
		
	}
	
	~InputLayer() override
	{
		
	}

	void OnUpdate() override
	{
		Input::GetInstance()->Listen();
	}
	
	void OnLateUpdate() override
	{
		Input::GetInstance()->Reset();
	}
};
