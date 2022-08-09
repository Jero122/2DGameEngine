#pragma once
#include "Core/SubSystem.h"

class ImGuiSystem : public SubSystem
{
public:
	enum Theme
	{
		Cherno,
		Unity,
		Unreal
	};
	
	ImGuiSystem() = default;
	~ImGuiSystem() = default;

	void OnStart() override;
	
	void OnEnd() override;
	void OnUpdate(TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;

	void SetDarkTheme(Theme theme);
	void Begin();
	void End();
};

