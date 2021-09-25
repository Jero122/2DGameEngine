#include "SceneHierarchyPanel.h"

#include "ECS/SceneView.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/Transform.h"
#include "imgui/imgui.h"


void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("Scene Hierarchy");

	for (auto ent : SceneView<Transform>(m_Scene->m_Ecs))
	{
		ImGui::Text("%s", m_Scene->m_Ecs.GetComponent<TagComponent>(ent)->tag.c_str());	
	}
	ImGui::End();
}


