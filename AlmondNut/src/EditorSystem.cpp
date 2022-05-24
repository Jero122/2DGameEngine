#include <random>

#include "EditorSystem.h"
#include <GL/glew.h>

#include "imgui_internal.h"
#include "ECS/Components/BoxCollider2D.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/ModelRendererComponent.h"
#include "ECS/Components/MovementComponent.h"
#include "imgui/imgui.h"
#include "Scenes/SceneSerializer.h"

EditorSystem::EditorSystem()
{
    m_FrameBufferSpec.width = 1280;
    m_FrameBufferSpec.height = 720;
    m_ViewportSize.x = 1280;
    m_ViewportSize.y = 720;
    CreateFrameBuffer(m_FrameBufferSpec);
}
std::default_random_engine generator;
std::uniform_real_distribution<float> randSpeed(0.1f, 1.0f);
std::uniform_real_distribution<float> randPositionX(-8.0f, 8.0f);
std::uniform_real_distribution<float> randPositionY(0.0f, 16.0f);
std::uniform_real_distribution<float> randPositionZ(-1.0f, 1.0f);
std::uniform_real_distribution<float> randRotation(0.0f, 90.0f);
std::uniform_real_distribution<float> randScale(0.8f, 1.5f);
std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);
std::uniform_real_distribution<float> randR(0,1.0f);
std::uniform_real_distribution<float> randG(0, 1.0f);
std::uniform_real_distribution<float> randB(0, 1.0f);

EditorSystem::~EditorSystem()
{
}

void EditorSystem::OnStart()
{
    m_EditorCamera = EditorCamera(45.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
    m_CurrentScene = std::make_shared<Scene>();
    m_SceneHierarchyPanel.SetScene(m_CurrentScene);

    m_PlayIcon = std::make_shared<Texture>("resources/textures/PlayButton.png");
    m_StopIcon = std::make_shared<Texture>("resources/textures/StopButton.png");
    std::shared_ptr<Texture> Crate = std::make_shared<Texture>("resources/textures/Crate.jpg");



    Entity floor = m_CurrentScene->CreateEntity("Floor");
    {
        auto transformComponent = floor.GetComponent<Transform>();
        *transformComponent = Transform{ glm::vec3(0,-3.5,0), glm::vec3(0,0,0),glm::vec3(16,1,1) };

        floor.AddComponent(SpriteRenderer{ 16, 1, {1,1,1,1.0f} });

        RigidBody rb = RigidBody{};
        rb.FixedRotation = false;
        rb.Type = RigidBody::BodyType::Static;
        floor.AddComponent(rb);

        BoxCollider2D collider = BoxCollider2D{ {0.0f,0.0f}, {0.5f, 0.5f} };
        floor.AddComponent(collider);
    }

    Entity enttA = m_CurrentScene->CreateEntity("crate");
    {
        auto transformComponent = enttA.GetComponent<Transform>();
        *transformComponent = Transform{ glm::vec3(0,10,0), glm::vec3(0,0,0),glm::vec3(1,1,1) };

        enttA.AddComponent(SpriteRenderer{ 1, 1, Crate->GetTexID() });

        RigidBody rb = RigidBody{};
        rb.FixedRotation = false;
        rb.Type = RigidBody::BodyType::Dynamic;
        enttA.AddComponent(rb);

        BoxCollider2D collider = BoxCollider2D{ {0.0f,0.0f}, {0.5f, 0.5f} };
        collider.Friction = 0.1f;
        enttA.AddComponent(collider);
    }

    Entity backpack = m_CurrentScene->CreateEntity("backpack");
    {
        auto transformComponent = backpack.GetComponent<Transform>();
        *transformComponent = Transform{ glm::vec3(0,0,0), glm::vec3(0,0,0),glm::vec3(1,1,-1) };
        auto backpackModel = std::make_shared<Model>("Resources/Models/backpack/backpack.obj");
        backpack.AddComponent(ModelRendererComponent{backpackModel});
    }

    Entity pointlight = m_CurrentScene->CreateEntity("pointlight");
    {
        auto transformComponent = pointlight.GetComponent<Transform>();

        glm::vec3 position = { 2.3f, -3.3f, -4.0f };
        glm::vec3 ambient = { 0.05f, 0.05f, 0.05f };
        glm::vec3 diffuse = { 0.8f, 0.8f, 0.8f };
        glm::vec3 specular = { 1.0f, 1.0f, 1.0f};
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;


        *transformComponent = Transform{ position, glm::vec3(0,0,0),glm::vec3(0.1f,0.1f,0.1f) };
        LightComponent lightcomponent{ position, ambient,diffuse,specular,constant,linear,quadratic };
        pointlight.AddComponent(lightcomponent);
        auto cube = std::make_shared<Model>("Resources/Models/Cube.obj");
        pointlight.AddComponent(ModelRendererComponent{ cube });
    }
    /*SceneSerializer sceneSerializer(m_CurrentScene);
    sceneSerializer.Serialize("assets/scenes/Example.alm");*/

    m_SceneState = SceneState::Play;
    m_CurrentScene->OnRuntimeStart();
}

void EditorSystem::OnEnd()
{
}

void EditorSystem::OnUpdate(TimeStep timeStep)
{
	//Resizing
	if (m_FrameBufferSpec.width != m_ViewportSize.x || m_FrameBufferSpec.height != m_ViewportSize.y)
	{
        m_FrameBufferSpec.width = m_ViewportSize.x;
        m_FrameBufferSpec.height = m_ViewportSize.y;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
      
        m_EditorCamera.SetViewPortSize(m_ViewportSize.x, m_ViewportSize.y);
        gluPerspective(45.0,  m_ViewportSize.x/ m_ViewportSize.y, -1.0f, 1000.0f);
        glMatrixMode(GL_MODELVIEW);
        CreateFrameBuffer(m_FrameBufferSpec);
	}

	
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);


	switch (m_SceneState)
	{
		case SceneState::Play:
	    {
            m_CurrentScene->OnRuntimeUpdate(timeStep, m_EditorCamera);
			break;
	    }
        case SceneState::Edit:
        {
            m_CurrentScene->OnEditorUpdate(timeStep, m_EditorCamera);
            break;
        }
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void EditorSystem::OnImGuiRender()
{
   //ImGui::ShowDemoWindow();
    bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static bool show_sceneHierarchy = true;
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
            ImGui::MenuItem("Scene Hierarchy", NULL, &show_sceneHierarchy);
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


    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,2 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });

    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];

    ImGui::PushStyleColor(ImGuiCol_Button,{0,0,0,0});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{buttonHovered.x,buttonHovered.y,buttonHovered.z, 0.5f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ buttonActive.x,buttonActive.y,buttonActive.z, 0.5f });

    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        std::shared_ptr<Texture> icon = m_SceneState == SceneState::Edit ? m_PlayIcon : m_StopIcon;
        float size = ImGui::GetWindowHeight() - 4.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton((ImTextureID)icon->GetTexID(), { size, size }, { 0,0 }, { 1,1 }, 0))
	    {
		    if (m_SceneState == SceneState::Edit)
		    {
                m_SceneState = SceneState::Play;
                m_CurrentScene->OnRuntimeStart();
		    }
            else if (m_SceneState == SceneState::Play)
		    {
                m_SceneState = SceneState::Edit;
                m_CurrentScene->OnRuntimeStop();
		    }
	    }
        ImGui::End();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);

    /*auto stats = GPUBatched_Renderer::GetStats();
    ImGui::Begin("Renderer Stats");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();
    GPUBatched_Renderer::ResetStats();*/

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("ViewPort");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    ImGui::Image(reinterpret_cast<void*>(m_FrameBuffer), {m_ViewportSize.x,m_ViewportSize.y}, {0,1}, {1,0});
    ImGui::End();
    ImGui::PopStyleVar();

   if (show_sceneHierarchy)
   {
       m_SceneHierarchyPanel.OnImGuiRender();
   }

    ImGui::Begin("Asset Browser");
    ImGui::End();

   {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowBgAlpha(0.35f);
        ImGui::Begin("Application Stats", &p_open, window_flags);
        m_TotalFPS += ImGui::GetIO().Framerate / 2.0f;
        m_TotalFrames++;
        m_ElapsedTime += ImGui::GetIO().DeltaTime;

        float currentFPS = 1.0f / ImGui::GetIO().DeltaTime;
        float currentFrameTime = 1000.0f / currentFPS;
        float avgFPS = ImGui::GetIO().Framerate;
        float avgFrameTime = 1000.0f / ImGui::GetIO().Framerate;

        /*if (m_ElapsedTime >= 15.0f)
        {
            m_ElapsedTime = 0.0f;
        }*/

        ImGui::Text("Application current %.3f ms/frame (%.1f FPS)", currentFrameTime, currentFPS);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", avgFrameTime, avgFPS);
        ImGui::Text("Elapsed Time: %.3f s", m_ElapsedTime);
        ImGui::End();
   }

	
    ImGui::End();
}

void EditorSystem::OnLateUpdate()
{
}

void EditorSystem::CreateFrameBuffer(FrameBufferSpec spec)
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

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_ViewportSize.x, m_ViewportSize.y); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

	
    //check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}