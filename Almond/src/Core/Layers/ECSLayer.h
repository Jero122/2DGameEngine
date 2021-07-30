#pragma once
#include "Core/Layer.h"

class ECSLayer : public Layer
{
public:
	ECSLayer();
	~ECSLayer() override;
	
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	
	void OnImGuiRender() override;
	void OnLateUpdate() override;
};

