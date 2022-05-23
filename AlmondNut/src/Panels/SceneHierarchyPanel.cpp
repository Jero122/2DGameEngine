#include "SceneHierarchyPanel.h"

#include "ECS/SceneView.h"
#include "ECS/Components/BoxCollider2D.h"
#include "ECS/Components/ModelRendererComponent.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/SpriteRenderer.h"
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
	for (auto ent : SceneView<TagComponent>(m_Scene->m_Ecs))
	{
		Entity entity{ent, m_Scene.get()};
		DrawEntityNode(entity);
	}

	if (ImGui::BeginPopupContextWindow(0,1,false))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			m_Scene->CreateEntity("Empty Entity");
		}
		ImGui::EndPopup();
	}
	ImGui::End();

	ImGui::Begin("Properties");
	DrawEntityProperties(m_SelectedEntity);

	if (m_SelectedEntity.IsValid())
	{
		std::string button = "Add Component";
		float font_size = ImGui::GetFontSize() * button.size() / 2;
		ImGui::Spacing();
		ImGui::SameLine(
			ImGui::GetContentRegionAvail().x / 2 -
			font_size + (font_size / 2)
		);
		
		if (ImGui::Button(button.c_str()))
			ImGui::OpenPopup("Add Component");
		
		if (ImGui::BeginPopup("Add Component"))
		{
			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectedEntity.AddComponent<SpriteRenderer>({ 50.0f,50.0f, {1,1,1,1} });
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Rigid Body"))
			{
				m_SelectedEntity.AddComponent<RigidBody>({});
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	
	
	ImGui::End();
	
	ImGui::ShowDemoWindow();
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity.HasComponent<T>())
	{
		auto io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		auto component = entity.GetComponent<T>();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0,3 });
		ImGui::PushFont(boldFont);
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopFont();
		ImGui::PopStyleVar();

		bool componentRemoved = false;
		if (ImGui::BeginPopupContextItem(0))
		{
			if (ImGui::MenuItem("Remove Component"))
			{
				componentRemoved = true;
			}
			ImGui::EndPopup();
		}
		
		if (open)
		{
			uiFunction(component, entity);
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (componentRemoved)
		{
			entity.RemoveComponent<T>();
		}
	}

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

static void DrawVec2Control(const std::string label, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f)
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
	ImGui::DragFloat("##X", &values.x, 0.1f, 0, 0, "%.2f");
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

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Deleted Entity"))
		{
			entityDeleted = true;
		}
		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_Scene->DestroyEntity(entity);
		if (m_SelectedEntity == entity)
		{
			m_SelectedEntity = {};
		}
	}
}

void SceneHierarchyPanel::DrawEntityProperties(Entity entity)
{
	if (entity.IsValid())
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];
			
			auto& tag = entity.GetComponent<TagComponent>()->tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, tag.c_str());

			ImGui::PushFont(boldFont);
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::PopFont();
		}

		DrawComponent<Transform>("Transform", entity, [](auto& component, Entity entity)
		{
			auto transform = (Transform*)component;
			DrawVec3Control("Position", transform->position);
			DrawVec3Control("Rotation", transform->rotation);
			DrawVec3Control("Scale", transform->scale, 1.0f);
		});

		DrawComponent<SpriteRenderer>("Sprite Renderer", entity, [](auto& component, Entity entity)
		{
			auto spriteRender = (SpriteRenderer*)component;
			ImGui::ColorEdit4("Color", glm::value_ptr(spriteRender->color));
		});

		DrawComponent<RigidBody>("RigidBody", entity, [](auto& component, Entity entity)
		{
			auto rb = (RigidBody*)component;

			const char* BodyTypeStrings[] = {"Static","Kinematic", "Dynamic"};
			const char* currentBodyTypeString = BodyTypeStrings[(int)rb->Type];

			if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i < IM_ARRAYSIZE(BodyTypeStrings); i++)
				{
					const bool is_selected = (currentBodyTypeString == BodyTypeStrings[i]);
					if (ImGui::Selectable(BodyTypeStrings[i], is_selected))
					{
						rb->Type = (RigidBody::BodyType)i;

					}
					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &(rb->FixedRotation));
			
		});

		DrawComponent<BoxCollider2D>("Box Collider 2D", entity, [](auto& component, Entity entity)
			{
				auto collider = (BoxCollider2D*)component;
				auto rb = entity.GetComponent<RigidBody>();

				DrawVec2Control("Offset", collider->Offset);
				DrawVec2Control("Size", collider->Size);

				ImGui::DragFloat("Density", &(collider->Density), 0.1f);
				ImGui::DragFloat("Friction", &(collider->Friction), 0.1f, 0, 1);
				ImGui::DragFloat("Restitution", &(collider->Restitution), 0.1f, 0, 1);
				ImGui::DragFloat("Restitution Threshold", &(collider->RestitutionThreshold), 0.1f, 0, 1);
			});
		DrawComponent<ModelRendererComponent>("Model Renderer", entity, [](auto& component, Entity entity)
			{
				auto model = (ModelRendererComponent*)component;
				ImGui::Text("Yes");
			});
	}
	
	
	
}
