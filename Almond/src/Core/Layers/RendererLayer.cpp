#include "RendererLayer.h"

#include <GL/glew.h>

#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECS.hpp"
#include "Core/Renderer/Renderer2D.h"
#include "imgui/imgui.h"

extern ECS ecs;
std::shared_ptr<Renderer2D> renderer;

RendererLayer::RendererLayer()
{
    renderer = ecs.CreateSystem<Renderer2D>();
    {
        Signature signature;
        signature.set(ecs.GetComponentType<SpriteRender>());
        signature.set(ecs.GetComponentType<Transform>());
        ecs.SetSystemSignature<Renderer2D>(signature);
    }

    renderer->init();
}

RendererLayer::~RendererLayer()
{
	
}

void RendererLayer::OnAttach()
{
	
}

void RendererLayer::OnDetach()
{
    renderer->shutdown();
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
    RenderStats stats = renderer->getRenderStats();
    bool batching = renderer->isBatching();

    ImGui::Begin("Renderer Stats");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    if (ImGui::Checkbox("use Batching", &batching))
    {
        renderer->setBatching(batching);
    }
    ImGui::End();

    ImGui::Begin("ECS");
    ImGui::Text("Living Entities: %d", ecs.getEntityManager()->mLivingEntityCount);
    ImGui::End();
}

void RendererLayer::OnLateUpdate()
{
}
