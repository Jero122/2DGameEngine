#pragma once
#include "Core/Layer.h"

class ImGuiLayer : public Layer
{
public:
	enum Theme
	{
		Cherno,
		Unity,
		Unreal
	};
	
	ImGuiLayer()
	{
		
	}

	~ImGuiLayer() override;
	void OnAttach() override;
	
	void OnDetach() override;
	void OnUpdate(TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;

	void SetDarkTheme(Theme theme);
	void Begin();
	void End();
};

