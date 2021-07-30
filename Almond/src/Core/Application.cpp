#include "Application.h"

#include "ECS/ECSLayer.h"
#include "Layers/InputLayer.h"
#include "Renderer/RendererLayer.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	s_Instance = this;
	m_Window = Window::Create(WindowProps());

	//INPUT
	InputLayer* input = new InputLayer();
	m_LayerStack.PushLayer(input);

	//ECS
	ECSLayer* ECS = new ECSLayer();
	m_LayerStack.PushLayer(ECS);
	
	//RENDERER
	RendererLayer* Renderer = new RendererLayer();
	m_LayerStack.PushLayer(Renderer);

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
		for (auto layer : m_LayerStack)
		{
			layer->OnUpdate();
		}

		m_ImGuiLayer->Begin();
		for (auto layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}

		for (auto layer : m_LayerStack)
		{
			layer->OnLateUpdate();
		}
		
		m_ImGuiLayer->End();
		m_Window->OnUpdate();
	}
}
