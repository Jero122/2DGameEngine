#pragma once
#include <memory>
#include <stack>
#include "Core/Layer.h"
#include "ECS/ECSTypes.h"
#include "ECS/Scene.h"
#include "glm/vec2.hpp"
#include "Panels/SceneHierarchyPanel.h"


class EditorLayer : public Layer
{
public:
	EditorLayer();
	~EditorLayer() override;
	
	void OnAttach() override;
	void OnDetach() override;
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
};

