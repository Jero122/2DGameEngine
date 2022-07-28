#include "Window.h"

#include "Platforms/Windows/WindowsWindow.h"


	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}

