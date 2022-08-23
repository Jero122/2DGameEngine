#include "AlmondApplication.h"
#include "TimeStep.h"
#include "Systems/ImGuiSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/FileSystem.h"
#include "Systems/TextureSystem.h"

AlmondApplication* AlmondApplication::s_Instance = nullptr;
int s_componentCounter = 0;

AlmondApplication::AlmondApplication()
{
	s_Instance = this;
	m_Window = Window::Create(WindowProps());
	
	//INPUT
	InputSystem* input = new InputSystem();
	m_SystemStack.PushSystem(input);

	FileSystem* fileSystem = new FileSystem();
	m_SystemStack.PushSystem(fileSystem);

	TextureSystem* textureSystem = new TextureSystem(65536);
	m_SystemStack.PushSystem(textureSystem);

	//IMGUI
	m_ImGuiSystem = new ImGuiSystem();
	m_SystemStack.PushOverLay(m_ImGuiSystem);

	for (auto system : m_SystemStack)
	{
		system->OnStart();
	}
}

AlmondApplication::~AlmondApplication()
{
	//TODO destruction of application
}

void AlmondApplication::PushSystem(SubSystem* system)
{
	m_SystemStack.PushSystem(system);
	system->OnStart();
}

void AlmondApplication::PushOverlay(SubSystem* overlay)
{
	m_SystemStack.PopOverlay(overlay);
	overlay->OnStart();
}

void AlmondApplication::Close()
{
	m_Running = false;
}

void AlmondApplication::Run()
{
	m_Running = true;
	while (m_Running)
	{
		float start = (float)SDL_GetTicks() / 1000.0f;
		TimeStep time = start - m_TimeSinceLastFrame;
		m_TimeSinceLastFrame = start;

		float fps = 1.0f / (time.GetSeconds());

		for (auto system : m_SystemStack)
		{
			system->OnUpdate(time);
		}

		m_ImGuiSystem->Begin();
		for (auto system : m_SystemStack)
		{
			system->OnImGuiRender();
		}

		m_ImGuiSystem->End();

		for (auto system : m_SystemStack)
		{
			system->OnLateUpdate();
		}
		m_Window->OnUpdate();
		

		
	}
}
