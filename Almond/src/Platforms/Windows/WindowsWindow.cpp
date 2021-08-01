#include "WindowsWindow.h"

#include <GL/glew.h>

WindowsWindow::WindowsWindow(const WindowProps& props)
{
	Init(props);
}

WindowsWindow::~WindowsWindow()
{
}

void WindowsWindow::OnUpdate()
{
	SDL_GL_SwapWindow(m_window);
}

void WindowsWindow::Init(const WindowProps& props)
{
	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Data.Width, m_Data.Height, SDL_WINDOW_OPENGL);

	gl_context = SDL_GL_CreateContext(m_window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW DIDNT INIT";
	}


	//glEnable(GL_MULTISAMPLE);
	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void WindowsWindow::ShutDown()
{
	SDL_DestroyWindow(m_window);
}

