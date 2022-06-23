#include "EditorSystem.h"

#include <random>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <Math/Math.h>

#include "ECS/Components/BoxCollider2D.h"
#include "ECS/Components/LightComponent.h"
#include "ECS/Components/ModelRendererComponent.h"
#include "ECS/Components/MovementComponent.h"

#include "Scenes/SceneSerializer.h"

#include "iostream"

EditorSystem::EditorSystem()
{
    m_ViewportSize.x = 1280;
    m_ViewportSize.y = 720;

    m_GLFrameBuffer = std::make_unique<GLFrameBuffer>(m_ViewportSize.x, m_ViewportSize.y);
    m_GLFrameBuffer->Bind();
	m_GLFrameBuffer->AddColourAttachment(GL_RGB8, GL_LINEAR);
    m_GLFrameBuffer->AddColourAttachment(GL_RGBA16I, GL_NEAREST);
	//m_GLFrameBuffer->AddColourAttachment(GL_R8I, GL_NEAREST);
    m_GLFrameBuffer->AddDepthAttachment();
    if (glCheckNamedFramebufferStatus(m_GLFrameBuffer->ID(), GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    m_GLFrameBuffer->UnBind();
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

    m_TranslateIcon = std::make_shared<Texture>("resources/textures/Translate.png");
    m_RotateIcon = std::make_shared<Texture>("resources/textures/Rotate.png");
    m_ScaleIcon = std::make_shared<Texture>("resources/textures/Scale.png");

    std::shared_ptr<Texture> Crate = std::make_shared<Texture>("resources/textures/Crate.jpg");



    /*Entity floor = m_CurrentScene->CreateEntity("Floor");
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

        enttA.AddComponent(SpriteRenderer{ 1, 1, Crate->ID() });

        RigidBody rb = RigidBody{};
        rb.FixedRotation = false;
        rb.Type = RigidBody::BodyType::Dynamic;
        enttA.AddComponent(rb);

        BoxCollider2D collider = BoxCollider2D{ {0.0f,0.0f}, {0.5f, 0.5f} };
        collider.Friction = 0.1f;
        enttA.AddComponent(collider);
    }*/

    Entity backpack = m_CurrentScene->CreateEntity("backpack");
    {
        auto transformComponent = backpack.GetComponent<Transform>();
        *transformComponent = Transform{ glm::vec3(0,0,0), glm::vec3(0,0,0),glm::vec3(1,1,-1) };
        auto backpackModel = std::make_shared<Model>("Resources/Models/backpack/backpack.obj");
        backpack.AddComponent(ModelRendererComponent{backpackModel});
    }

    Entity pointlight1 = m_CurrentScene->CreateEntity("pointlight");
    {
        auto transformComponent = pointlight1.GetComponent<Transform>();

        glm::vec3 position = { 1.05, 0.0f, -1.0f };
        glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
        glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
        glm::vec3 specular = { 1.0f, 1.0f, 1.0f};
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;


        *transformComponent = Transform{ position, glm::vec3(0,0,0),glm::vec3(0.1f,0.1f,0.1f) };
        LightComponent lightcomponent{position,ambient,diffuse,specular,constant,linear,quadratic};
        pointlight1.AddComponent(lightcomponent);
        auto cube = std::make_shared<Model>("Resources/Models/Cube.obj");
        pointlight1.AddComponent(ModelRendererComponent{ cube });
    }

    Entity pointlight2 = m_CurrentScene->CreateEntity("pointlight");
    {
        auto transformComponent = pointlight2.GetComponent<Transform>();

        glm::vec3 position = { -1.05, 0.0f, -1.0f };
        glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
        glm::vec3 diffuse = { 0.5f, 0.5f, 0.5f };
        glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;


        *transformComponent = Transform{ position, glm::vec3(0,0,0),glm::vec3(0.1f,0.1f,0.1f) };
        LightComponent lightcomponent{ position,ambient,diffuse,specular,constant,linear,quadratic };
        pointlight2.AddComponent(lightcomponent);
        auto cube = std::make_shared<Model>("Resources/Models/Cube.obj");
        pointlight2.AddComponent(ModelRendererComponent{ cube });
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
	if (m_GLFrameBuffer->GetWidth() != m_ViewportSize.x || m_GLFrameBuffer->GetHeight() != m_ViewportSize.y)
	{
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
      
        m_EditorCamera.SetViewPortSize(m_ViewportSize.x, m_ViewportSize.y);
        gluPerspective(45.0,  m_ViewportSize.x/ m_ViewportSize.y, -1.0f, 1000.0f);
        glMatrixMode(GL_MODELVIEW);

        m_GLFrameBuffer->Bind();
        m_GLFrameBuffer->Invalidate(m_ViewportSize.x, m_ViewportSize.y);
    }

    m_GLFrameBuffer->Bind();

    GLRenderCommand::ClearColor(62.0f / 255.0f, 62.0f / 255.0f, 58.0f / 255.0f, 1.0f);
    GLRenderCommand::Clear();

    m_GLFrameBuffer->ClearColourAttachment(1, -1);
 
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

    auto Input = Input::GetInstance();

    //Mouse Picking
	if (Input->GetMouseButtonDown(Input::MouseButton::left))
	{
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        int pixel = m_GLFrameBuffer->ReadColourAttachment(1, mouseX, mouseY);
        std::cout << pixel << "\n";

        if (pixel <= -1)
        {
            m_HoveredEntity = Entity();
        }
        else
        {
            m_HoveredEntity = Entity{(EntityID)pixel, m_CurrentScene.get() };
            m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        }
	}

    //Transformation Mode
    //Translation
	if (Input->GetKeyDown(SDL_SCANCODE_Q))
	{
        m_TranformationMode = ImGuizmo::OPERATION::TRANSLATE;
	}
    //Rotation
    if (Input->GetKeyDown(SDL_SCANCODE_W))
    {
        m_TranformationMode = ImGuizmo::OPERATION::ROTATE;
    }
    //Scale
    if (Input->GetKeyDown(SDL_SCANCODE_E))
    {
        m_TranformationMode = ImGuizmo::OPERATION::SCALE;
    }
  
    m_GLFrameBuffer->UnBind();
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
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
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

    auto& colors = ImGui::GetStyle().Colors;
    const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
    const auto& buttonActive = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,2 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });
    ImGui::PushStyleColor(ImGuiCol_Button,{0,0,0,0});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{buttonHovered.x,buttonHovered.y,buttonHovered.z, 0.5f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ buttonActive.x,buttonActive.y,buttonActive.z, 0.5f });


    ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);
    {
        std::shared_ptr<Texture> icon = m_SceneState == SceneState::Edit ? m_PlayIcon : m_StopIcon;
        float size = ImGui::GetWindowHeight() - 4.0f;

		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->ID(), { size, size }, { 0,0 }, { 1,1 }, 0))
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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,0 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });
    ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ buttonHovered.x,buttonHovered.y,buttonHovered.z, 0.5f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ buttonActive.x,buttonActive.y,buttonActive.z, 0.5f });

    ImGui::Begin("##GuizmoBar", nullptr,  ImGuiWindowFlags_NoDecoration |ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
        ImGui::SetNextWindowBgAlpha(0.35f);
        float size = ImGui::GetWindowWidth();

        if (ImGui::ImageButton((ImTextureID)m_TranslateIcon->ID(), { size, size }, { 0,0 }, { 1,1 }, 0))
        {
            m_TranformationMode = ImGuizmo::OPERATION::TRANSLATE;
        }
   
        if (ImGui::ImageButton((ImTextureID)m_RotateIcon->ID(), { size, size }, { 0,0 }, { 1,1 }, 0))
        {
            m_TranformationMode = ImGuizmo::OPERATION::ROTATE;
        }
    
        if (ImGui::ImageButton((ImTextureID)m_ScaleIcon->ID(), { size, size }, { 0,0 }, { 1,1 }, 0))
        {
            m_TranformationMode = ImGuizmo::OPERATION::SCALE;
        }

        ImGui::End();
    }
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("ViewPort");
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
    m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
    ImGui::Image(reinterpret_cast<void*>(m_GLFrameBuffer->ID()), { m_ViewportSize.x,m_ViewportSize.y }, { 0,1 }, { 1,0 });

    //Gizmos
    Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
	if (selectedEntity)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

        auto tc = selectedEntity.GetComponent<Transform>();
        glm::mat4 transform = tc->GetTransform();

        const glm::mat4& projection = m_EditorCamera.GetProjectionMatrix();
        glm::mat4 view = m_EditorCamera.GetViewMatrix();


        float tmpMatrix[16];
        ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(tc->position), glm::value_ptr(tc->rotation), glm::value_ptr(tc->scale), tmpMatrix);
        ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), m_TranformationMode,
            ImGuizmo::MODE::LOCAL, tmpMatrix);


        if (ImGuizmo::IsUsing())
        {
            float matrixTranslation[3], matrixRotation[3], matrixScale[3];
            ImGuizmo::DecomposeMatrixToComponents(tmpMatrix, matrixTranslation, matrixRotation, matrixScale);

            glm::vec3 deltaRotation = glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]) - tc->rotation;
            tc->position = glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
            tc->rotation += deltaRotation;
            tc->scale = glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]);
        }
	}

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