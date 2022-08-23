#pragma once
#include "SystemStack.h"
#include "Window.h"
#include "Systems/ImGuiSystem.h"


class AlmondApplication
{
public:
	AlmondApplication();
	virtual ~AlmondApplication();

	static AlmondApplication& Get() { return *s_Instance; }
	Window& GetWindow() { return *m_Window; }
	ImGuiSystem* GetImGuiSystem() { return m_ImGuiSystem; }


	void PushSystem(SubSystem* system);
	void PushOverlay(SubSystem* overlay);

	void Close();
	void Run();
private:

	static AlmondApplication* s_Instance;

	bool m_Running = false;
	float m_TimeSinceLastFrame = 0.0f;

	SystemStack m_SystemStack;
	Scope<Window> m_Window;
	ImGuiSystem* m_ImGuiSystem;

};

