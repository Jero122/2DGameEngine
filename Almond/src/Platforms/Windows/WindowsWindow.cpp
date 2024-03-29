#include "WindowsWindow.h"
#define GLEW_STATIC
#include <GL/glew.h>

#include "Core/Log.h"

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


	m_window = SDL_CreateWindow(m_Data.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_Data.Width, m_Data.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	
	gl_context = SDL_GL_CreateContext(m_window);
	if (glewInit() != GLEW_OK)
	{
		AL_ENGINE_CRITICAL("GLEW DID NOT INITIALISE");
	}


	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);



	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void WindowsWindow::ShutDown()
{
	SDL_DestroyWindow(m_window);
}

