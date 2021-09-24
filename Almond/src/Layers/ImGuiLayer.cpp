#include "ImGuiLayer.h"

#include <imgui/imgui_impl_opengl3_loader.h>

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

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	/*glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);*/
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}
