#include "Application.h"
#include "TimeStep.h"
#include "Editor/EditorLayer.h"
#include "imgui/imgui.h"
#include "Layers/InputLayer.h"
#include "Physics2D/PhysicsWorld.h"
#include "Renderer/Renderer2D.h"

Application* Application::s_Instance = nullptr;
int s_componentCounter = 0;

Application::Application()
{
	s_Instance = this;
	m_Window = Window::Create(WindowProps());

	PhysicsWorld::GetInstance()->SetGravity({0,-10});
	
	//INPUT
	InputLayer* input = new InputLayer();
	m_LayerStack.PushLayer(input);

	//ECS
	EditorLayer* Editor = new EditorLayer();
	m_LayerStack.PushLayer(Editor);
	
	Renderer2D::Init();
	
	//IMGUI
	m_ImGuiLayer = new ImGuiLayer();
	m_LayerStack.PushOverLay(m_ImGuiLayer);

	for (auto layer : m_LayerStack)
	{
		layer->OnAttach();
	}
}

Application::~Application()
{
	//TODO destruction of application
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay)
{
	m_LayerStack.PopOverlay(overlay);
	overlay->OnAttach();
}

void Application::Close()
{
	m_Running = false;
}

void Application::Run()
{
	m_Running = true;
	while (m_Running)
	{
		float start = (float)SDL_GetTicks() / 1000.0f;
		TimeStep time = start - m_TimeSinceLastFrame;
		m_TimeSinceLastFrame = start;

		float fps = 1.0f / (time.GetSeconds());

		std::cout << "FrameTime: " << time.GetMilliseconds() << " | FPS: " << fps << std::endl;
	
		for (auto layer : m_LayerStack)
		{
			layer->OnUpdate(time);
		}

		m_ImGuiLayer->Begin();
		for (auto layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}


		m_ImGuiLayer->End();

		for (auto layer : m_LayerStack)
		{
			layer->OnLateUpdate();
		}
		m_Window->OnUpdate();
		

		
	}
}
