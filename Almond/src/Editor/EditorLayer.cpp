#include <random>

#include "EditorLayer.h"
#include "ECS/Entity.h"
#include "Core/Input.h"
#include "ECS/Components/SpriteRender.h"
#include "ECS/Components/Transform.h"
#include "Physics2D/Geometry2D.h"
#include "Renderer/SpriteSheet.h"
#include "Renderer/Texture.h"
#include "imgui/imgui.h"
#include "ECS/Components/RigidBody.h"
#include "Physics2D/Physics2D.h"
#include "Physics2D/PhysicsWorld.h"
#include "Renderer/OldCamera.h"
#include "Renderer/Renderer2D.h"




EditorLayer::EditorLayer()
{
    m_FrameBufferSpec.width = 1280;
    m_FrameBufferSpec.height = 720;
    m_ViewportSize.x = 1280;
    m_ViewportSize.y = 720;
    CreateFrameBuffer(m_FrameBufferSpec);
}
std::default_random_engine generator;
std::uniform_real_distribution<float> randPositionX(-8.0f, 8.0f);
std::uniform_real_distribution<float> randPositionY(0.0f, 8.0f);
std::uniform_real_distribution<float> randRotation(0.0f, 90.0f);
std::uniform_real_distribution<float> randScale(0.8f, 1.5f);
std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnAttach()
{
    Texture texture("resources/textures/container.jpg");
    SpriteSheet spriteSheet("resources/textures/UpArrow.png", 32, 32, 1, 1);
    Texture texture1("resources/textures/Crate.jpg");

    m_CurrentScene = std::make_shared<Scene>();
    m_SceneHierarchyPanel.SetScene(m_CurrentScene);


    Entity floor = m_CurrentScene->CreateEntity("Floor");
    {
        auto transformComponent = floor.GetComponent<Transform>();
        *transformComponent = Transform{ glm::vec3(0,-4.5,0), glm::vec3(0,0,0),glm::vec3(16,1,1) };

        floor.AddComponent(SpriteRender{ 16, 1, {255,255,255,1} });

        RigidBody body(*PhysicsWorld::GetInstance(), 0, -4.5, BodyType::Static);
        OrientedBox box(8, 0.50, 0.0f, 1.0f, 0.0f);

        body.AddBoxCollider(box);
        floor.AddComponent(body);
    }
	
    for (int i = 0; i < 10; ++i)
    {
        std::string name = "Crate ";
        name += std::to_string(i);
        auto entity = m_CurrentScene->CreateEntity(name);
        {
            auto pos = glm::vec3{ randPositionX(generator),randPositionY(generator),0.0f };
            auto rot = glm::vec3{ 0.0f,0.0f,0 };
            auto scale = glm::vec3{ 0.5f,0.5f,1.0f };
        	
            auto transform = entity.GetComponent<Transform>();
        	*transform = Transform{ glm::vec3(pos.x,pos.y, 0),rot,scale };
        	
            entity.AddComponent(SpriteRender{ 0.5f , 0.5f, texture1.GetTexID() });

            RigidBody body(*PhysicsWorld::GetInstance(), pos.x, pos.y, BodyType::Dynamic);
            OrientedBox box(0.24, 0.24, 1.0f, 0.1f, 0.0f);
            body.AddBoxCollider(box);
        	
            entity.AddComponent(body);
        }
    }
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(TimeStep timeStep)
{
	//Resizing
	if (m_FrameBufferSpec.width != m_ViewportSize.x || m_FrameBufferSpec.height != m_ViewportSize.y)
	{
        m_FrameBufferSpec.width = m_ViewportSize.x;
        m_FrameBufferSpec.height = m_ViewportSize.y;
        m_CurrentScene->GetEditorCamera().SetViewPortSize(m_ViewportSize.x, m_ViewportSize.y);

        CreateFrameBuffer(m_FrameBufferSpec);
	}

	
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
	
    m_CurrentScene->OnUpdate(timeStep);
	
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EditorLayer::OnImGuiRender()
{

   /*bool showDemo = true;
   ImGui::ShowDemoWindow(&showDemo);*/
	
    bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }


    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                p_open = false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    auto stats = Renderer2D::GetStats();
    ImGui::Begin("Renderer Stats");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();
    Renderer2D::ResetStats();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("ViewPort");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    ImGui::Image(reinterpret_cast<void*>(m_FrameBuffer), {m_ViewportSize.x,m_ViewportSize.y}, {0,1}, {1,0});
    ImGui::End();
    ImGui::PopStyleVar();

    m_SceneHierarchyPanel.OnImGuiRender();
 
    ImGui::Begin("Properties");
    ImGui::End();
	
    ImGui::Begin("Asset Browser");
    ImGui::End();

   {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Application Stats", &p_open, window_flags);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
   }

	
    ImGui::End();
}

void EditorLayer::OnLateUpdate()
{
}

void EditorLayer::CreateFrameBuffer(FrameBufferSpec spec)
{
	if (m_FrameBuffer != 0)
	{
        glDeleteFramebuffers(1, &m_FrameBuffer);
        glDeleteTextures(1, &m_ColourAttachment);
	}
	
    //FRAME BUFFER
    GLCALL(glGenFramebuffers(1, &m_FrameBuffer));
    GLCALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBuffer));
    //generate frame buffer texture

    glGenTextures(1, &m_ColourAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColourAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spec.width, spec.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourAttachment, 0);

    //check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}