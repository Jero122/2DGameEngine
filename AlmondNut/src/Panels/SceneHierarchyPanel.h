#pragma once
#include <memory>

#include "ECS/Entity.h"
#include "ECS/Scene.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const std::shared_ptr<Scene> scene);
	
	void SetScene(const std::shared_ptr<Scene>& scene) { m_Scene = scene; }
	void OnImGuiRender();

private:
	void DrawEntityNode(Entity entity);
	void DrawEntityProperties(Entity entity);
	
private:
	std::shared_ptr<Scene> m_Scene;
	Entity m_SelectedEntity;
};