#pragma once
#include <memory>
#include <stack>
#include "Almond.h"
#include "glm/vec2.hpp"
#include "Panels/SceneHierarchyPanel.h"
#include "Renderer/GLFramework/GLFrameBuffer.h"

#include "imgui/imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"
#include "Panels/AssetBrowserPanel.h"

class EditorSystem : public SubSystem
{
public:
	EditorSystem();
	~EditorSystem();
	
	void OnStart() override;
	void OnEnd() override;
	void OnUpdate(TimeStep timeStep) override;
	
	void OnImGuiRender() override;
	void OnLateUpdate() override;
	std::stack<EntityID> m_entities;
	std::shared_ptr<Scene> m_CurrentScene;
	int m_GizmoType = -1;

private:
	std::unique_ptr<GLFrameBuffer> m_GLFrameBuffer;

	glm::vec2 m_ViewportSize = {1280.0f, 720.0f};
	glm::vec2 m_ViewportBounds[2];
	bool m_ViewportHovered;
	bool m_ViewportFocused;

	SceneHierarchyPanel m_SceneHierarchyPanel;
	AssetBrowserPanel m_AssetBrowserPanel;

	EditorCamera m_EditorCamera;

	Entity m_HoveredEntity;

	enum class SceneState
	{
		Play = 0, Edit
	};

	SceneState m_SceneState = SceneState::Edit;

	ImGuizmo::OPERATION m_TranformationMode = ImGuizmo::OPERATION::TRANSLATE;

	float m_TotalFPS = 0.0f;
	float m_TotalFrames = 0.0f;
	float m_ElapsedTime = 0.0f;
};

