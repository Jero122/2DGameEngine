#pragma once
#include <string>

#include "TimeStep.h"

class Layer
{
public:



	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach(){}
	virtual void OnUpdate(TimeStep timeStep){}
	virtual void OnImGuiRender(){}
	virtual void OnLateUpdate(){}

	const std::string& GetName() { return m_debugName; }

protected:
	std::string m_debugName;
};

