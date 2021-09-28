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

	ImGui::Begin("Properties");
	DrawEntityProperties(m_SelectedEntity);
	ImGui::End();
	
	ImGui::ShowDemoWindow();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	auto tag = entity.GetComponent<TagComponent>()->tag;

	ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected: 0 |ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tag.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectedEntity = entity;
	}

	if (opened)
	{
		ImGui::TreePop();
	}
}

void SceneHierarchyPanel::DrawEntityProperties(Entity entity)
{
	if (entity.IsValid())
	{
		auto& tag = entity.GetComponent<TagComponent>()->tag;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, tag.c_str());

		if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}
	
	
	
}
