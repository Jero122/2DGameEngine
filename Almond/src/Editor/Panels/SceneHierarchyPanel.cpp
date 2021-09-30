#include "SceneHierarchyPanel.h"

#include "ECS/SceneView.h"
#include "ECS/Entity.h"
#include "ECS/Components/SpriteRender.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/Transform.h"
#include "glm/gtc/type_ptr.hpp"
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
		if (entity.HasComponent<Transform>())
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

		if (entity.HasComponent<Transform>())
		{
			//TODO change v_speed based on zoom level
			auto transform = entity.GetComponent<Transform>();
			if (ImGui::TreeNodeEx((void*)typeid(Transform).hash_code(),ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(transform->position), 0.1f);
				ImGui::DragFloat3("Rotation", glm::value_ptr(transform->rotation), 0.1f);
				ImGui::DragFloat3("Scale", glm::value_ptr(transform->scale), 0.1f);

				ImGui::TreePop();
			}
			
		}

		if (entity.HasComponent<SpriteRender>())
		{
			static bool ref_color = true;
			static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
			
			ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_AlphaPreview;
			//TODO change v_speed based on zoom level
			auto spriteRender = entity.GetComponent<SpriteRender>();
			auto& color = spriteRender->color;
			
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRender).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				ImGui::Text("Color");
				ImGui::SameLine();
				bool open_popup = ImGui::ColorButton("MyColor##3b", *(ImVec4*)&color, misc_flags, { 150,20 });
				ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
				if (open_popup)
				{
					ImGui::OpenPopup("mypicker");
				}
				if (ImGui::BeginPopup("mypicker"))
				{
					ImGui::Text("Color");
					ImGui::Separator();
		
					ImGuiColorEditFlags flags = misc_flags;
					flags |= ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaPreview;
					ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);
				}
				ImGui::TreePop();
			}

		}
		
	}
	
	
	
}
