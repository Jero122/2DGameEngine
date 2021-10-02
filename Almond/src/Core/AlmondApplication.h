#pragma once
#include "LayerStack.h"
#include "Window.h"
#include "Layers/ImGuiLayer.h"


class ALMOND_API AlmondApplication
{
public:
	AlmondApplication();
	virtual ~AlmondApplication();

	static AlmondApplication& Get() { return *s_Instance; }
	Window& GetWindow() { return *m_Window; }
	ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }


	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);

	void Close();
	void Run();
private:

	static AlmondApplication* s_Instance;

	bool m_Running = false;
	float m_TimeSinceLastFrame = 0.0f;

	LayerStack m_LayerStack;
	Scope<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;

};

