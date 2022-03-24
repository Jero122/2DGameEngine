#pragma once
#include <string>

#include "TimeStep.h"

class GameSystem
{
public:

	virtual ~GameSystem() = default;
	virtual void OnStart() {}
	virtual void OnEnd(){}
	virtual void OnUpdate(TimeStep timeStep){}
	virtual void OnImGuiRender(){}
	virtual void OnLateUpdate(){}
};

