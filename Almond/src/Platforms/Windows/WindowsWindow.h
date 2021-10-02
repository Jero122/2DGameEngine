#pragma once
#include "Core/Window.h"
#include "SDL/SDL.h"

class WindowsWindow : public Window
{
public:
	WindowsWindow(const WindowProps& props);
	virtual ~WindowsWindow();

	void OnUpdate() override;

	virtual void* GetNativeWindow() const { return m_window; }
	virtual void* GetGLContext() const { return gl_context; }

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
