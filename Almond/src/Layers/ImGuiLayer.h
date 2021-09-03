#pragma once
#include "Core/Layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer()
	{
		
	}

	~ImGuiLayer() override;
	void OnAttach() override;
	
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;

	void Begin();
	void End();
};

