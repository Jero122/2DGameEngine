#pragma once
#include <memory>
#include <stack>

#include "ECSTypes.h"
#include "Core/Layer.h"

#include "Scene.h";

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
	std::stack<EntityID> m_entities;
	std::shared_ptr<Scene> m_CurrentScene;
};

