#pragma once
#include <memory>
#include "ECS/Scene.h"

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	void SetScene(const std::shared_ptr<Scene>& scene) { m_Scene = scene; }
	
	void OnImGuiRender();

private:
	std::shared_ptr<Scene> m_Scene;
};
