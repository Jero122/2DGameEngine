#pragma once
#include <string>

#include "TimeStep.h"

class SubSystem
{
public:
	SubSystem() = default;
	virtual ~SubSystem() = default;

	virtual void OnStart() {}
	virtual void OnEnd(){}
	virtual void OnUpdate(TimeStep timeStep){}
	virtual void OnImGuiRender(){}
	virtual void OnLateUpdate(){}
};

