#include "ImGuiLayer.h"

#include "Core/Application.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	Application& app = Application::Get();
	auto* window = static_cast<SDL_Window*>(app.Get().GetWindow().GetNativeWindow());
	const auto glContext = app.Get().GetWindow().GetGLContext();

	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate(TimeStep timeStep)
{
}

void ImGuiLayer::OnImGuiRender()
{
}

void ImGuiLayer::OnLateUpdate()
{
}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow()));
	ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
