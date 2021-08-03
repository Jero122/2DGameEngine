#include "RendererLayer.h"

#include <GL/glew.h>

#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECS.hpp"
#include "Core/Renderer/RenderSysten2D.h"
#include "imgui/imgui.h"

extern ECS ecs;
std::shared_ptr<RenderSysten2D> renderer;

RendererLayer::RendererLayer()
{
    renderer = ecs.CreateSystem<RenderSysten2D>();
    {
        Signature signature;
        signature.set(ecs.GetComponentType<SpriteRender>());
        signature.set(ecs.GetComponentType<Transform>());
        ecs.SetSystemSignature<RenderSysten2D>(signature);
    }

    renderer->Init();
}

RendererLayer::~RendererLayer() = default;

void RendererLayer::OnAttach()
{
	
}

void RendererLayer::OnDetach()
{
    renderer->ShutDown();
}

void RendererLayer::OnUpdate()
{
    //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    renderer->Update();
}

void RendererLayer::OnImGuiRender()
{
    RenderStats stats = renderer->GetRenderStats();
    ImGui::Begin("Renderer Stats");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();
}

void RendererLayer::OnLateUpdate()
{
}
