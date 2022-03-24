#pragma once
#include <memory>
#include <stack>
#include "Almond.h"
#include "glm/vec2.hpp"
#include "Panels/SceneHierarchyPanel.h"


class EditorSystem : public GameSystem
{
public:
	EditorSystem();
	~EditorSystem() override;
	
	void OnStart() override;
	void OnEnd() override;
	void OnUpdate(TimeStep timeStep) override;
	
	void OnImGuiRender() override;
	void OnLateUpdate() override;
	std::stack<EntityID> m_entities;
	std::shared_ptr<Scene> m_CurrentScene;

private:
	struct FrameBufferSpec
	{
		float width;
		float height;
	};
	
	void CreateFrameBuffer(FrameBufferSpec spec);
	unsigned int m_FrameBuffer = 0;
	FrameBufferSpec m_FrameBufferSpec = {1280.0f, 720.0f};
	unsigned int m_ColourAttachment;
	glm::vec2 m_ViewportSize = {1280.0f, 720.0f};

	SceneHierarchyPanel m_SceneHierarchyPanel;
	EditorCamera m_EditorCamera;

	enum class SceneState
	{
		Play = 0, Edit
	};

	SceneState m_SceneState = SceneState::Edit;
	std::shared_ptr<Texture> m_PlayIcon;
	std::shared_ptr<Texture> m_StopIcon;

	float m_TotalFPS = 0.0f;
	float m_TotalFrames = 0.0f;
	float m_ElapsedTime = 0.0f;
};

