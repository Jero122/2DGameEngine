#include "Application.h"

#include "Components/RigidBody.h"
#include "ECS/ECS.hpp"
#include "ECS/ECSLayer.h"
#include "imgui/imgui.h"
#include "Layers/InputLayer.h"
#include "Layers/Physics2DLayer.h"
#include "Layers/RendererLayer.h"
#include "Renderer/RenderBatch.h"

Application* Application::s_Instance = nullptr;
ECS ecs;


Application::Application()
{
	s_Instance = this;
	m_Window = Window::Create(WindowProps());

	ecs.Init();

	ecs.CreateComponent<SpriteRender>();
	ecs.CreateComponent<Transform>();
	ecs.CreateComponent<RigidBody>();
	
	//PHYSICS
	Physics2DLayer* physics = new Physics2DLayer();
	m_LayerStack.PushLayer(physics);
	
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
		Uint64 start = SDL_GetPerformanceCounter();



	
		for (auto layer : m_LayerStack)
		{
			layer->OnUpdate();
		}

		m_ImGuiLayer->Begin();
		for (auto layer : m_LayerStack)
		{
			layer->OnImGuiRender();
		}

		ImGui::Begin("Application Stats");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		m_ImGuiLayer->End();

		for (auto layer : m_LayerStack)
		{
			layer->OnLateUpdate();
		}
		m_Window->OnUpdate();
		
		Uint64 end = SDL_GetPerformanceCounter();
		float secondsElapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
		float fps = 1.0f / secondsElapsed;
		//std::cout << "FrameTime: " << secondsElapsed * 1000.0f<< " | FPS: " << fps << std::endl;
	}
}
