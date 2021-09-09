#include "Application.h"

#include "TimeStep.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/ECS.hpp"
#include "ECS/ECSLayer.h"
#include "imgui/imgui.h"
#include "Layers/InputLayer.h"
#include "Layers/Physics2DLayer.h"
#include "Physics2D/PhysicsWorld.h"
#include "Renderer/RenderBatch.h"
#include "Renderer/Renderer2D.h"

Application* Application::s_Instance = nullptr;
ECS ecs;
Camera camera;
int s_componentCounter = 0;

Application::Application()
{
	s_Instance = this;
	m_Window = Window::Create(WindowProps());

	ecs.Init();

	ecs.CreateComponent<SpriteRender>();
	ecs.CreateComponent<Transform>();
	ecs.CreateComponent<RigidBody>();

	camera = Camera();
	camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	PhysicsWorld::GetInstance()->SetGravity({0,-10});
	
	
	//INPUT
	InputLayer* input = new InputLayer();
	m_LayerStack.PushLayer(input);

	//ECS
	ECSLayer* ECS = new ECSLayer();
	m_LayerStack.PushLayer(ECS);
	

	Renderer2D::Init();
	/*//RENDERER
	RendererLayer* Renderer = new RendererLayer();
	m_LayerStack.PushLayer(Renderer);
	
	//PHYSICS
	Physics2DLayer* physics = new Physics2DLayer();
	m_LayerStack.PushLayer(physics);*/
	
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

		ImGui::Begin("Application Stats");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		m_ImGuiLayer->End();

		for (auto layer : m_LayerStack)
		{
			layer->OnLateUpdate();
		}
		m_Window->OnUpdate();
		

		
	}
}
