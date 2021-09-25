#include "SceneHierarchyPanel.h"

#include "ECS/SceneView.h"
#include "ECS/Entity.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/Transform.h"
#include "imgui/imgui.h"


SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene> scene)
	:m_Scene(scene)
{
}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");

	for (auto ent : SceneView<Transform>(m_Scene->m_Ecs))
	{
		Entity entity{ent, m_Scene.get()};
		DrawEntityNode(entity);
	}
	ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	ImGui::Text("%s", entity.GetComponent<TagComponent>()->tag.c_str());
}