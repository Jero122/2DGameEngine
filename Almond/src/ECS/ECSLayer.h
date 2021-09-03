#pragma once
#include <stack>

#include "ECSTypes.hpp"
#include "Core/Layer.h"

class ECSLayer : public Layer
{
public:
	ECSLayer();
	~ECSLayer() override;
	
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(TimeStep timeStep) override;
	
	void OnImGuiRender() override;
	void OnLateUpdate() override;
	std::stack<Entity> m_entities;
};

