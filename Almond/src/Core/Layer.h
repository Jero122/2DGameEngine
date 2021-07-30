#pragma once
#include <string>

class Layer
{
public:



	virtual ~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach(){}
	virtual void OnUpdate(){}
	virtual void OnImGuiRender(){}
	virtual void OnLateUpdate(){}

	const std::string& GetName() { return m_debugName; }

protected:
	std::string m_debugName;
};

