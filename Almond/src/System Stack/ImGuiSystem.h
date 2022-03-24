#pragma once
#include "Core/GameSystem.h"

class ImGuiSystem : public GameSystem
{
public:
	enum Theme
	{
		Cherno,
		Unity,
		Unreal
	};
	
	ImGuiSystem()
	{
		
	}

	~ImGuiSystem() override;
	void OnStart() override;
	
	void OnEnd() override;
	void OnUpdate(TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;

	void SetDarkTheme(Theme theme);
	void Begin();
	void End();
};

