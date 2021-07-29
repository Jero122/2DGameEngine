#pragma once
#include "Base.h"
#include "LayerStack.h"
#include "Window.h"

class Application
{
public:
	Application();
	virtual ~Application();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	
	void close();
	void Run();
private:
	


	bool m_Running = false;
	
	LayerStack m_LayerStack;
	Scope<Window> m_Window;
};

