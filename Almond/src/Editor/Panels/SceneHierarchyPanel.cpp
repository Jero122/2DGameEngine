#include "SceneHierarchyPanel.h"

#include "ECS/SceneView.h"
#include "ECS/Entity.h"
#include "ECS/Components/SpriteRender.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/Transform.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"


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

static void DrawVec3Control(const std::string label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGui::PushID(label.c_str());
	
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0,1.0f });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { 5, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0.1f,0.15f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f,0.2f,0.2f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f,0.1f,0.15f,1.0f });
	if (ImGui::Button("##0", buttonSize))
		values.x = resetValue;
	ImGui::PopStyleColor(3);
	
	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f,0,0, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.2f,0.7f,0.2f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f,0.8f,0.3f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f,0.7f,0.2f,1.0f });
	if (ImGui::Button("##1", buttonSize))
		values.y = resetValue;
	ImGui::PopStyleColor(3);
	
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0, 0, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f,0.25f,0.8f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.2f,0.35f,0.9f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f,0.25f,0.8f,1.0f });
	if (ImGui::Button("##2", buttonSize))
		values.z = resetValue;
	ImGui::PopStyleColor(3);
	
	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0, 0, "%.2f");
	ImGui::PopItemWidth();

	
	ImGui::PopStyleVar();
	ImGui::Columns(1);
	ImGui::PopID();
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
		if (entity.HasComponent<TagComponent>())
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
				DrawVec3Control("Position", transform->position);
				DrawVec3Control("Rotation", transform->rotation);
				DrawVec3Control("Scale", transform->scale, 1.0f);
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
