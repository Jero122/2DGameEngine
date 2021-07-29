#pragma once
#include "Core/Window.h"

class WindowsWindow : public Window
{
public:
	WindowsWindow(const WindowProps& props);
	virtual ~WindowsWindow();

	void OnUpdate() override;

private:
	virtual void Init(const WindowProps& props);
	virtual void ShutDown();

	SDL_Window* m_window;
	SDL_GLContext gl_context;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool Vsync;
	};

	WindowData m_Data;
};
