#include "SceneHierarchyPanel.h"

#include "ECS/SceneView.h"
#include "ECS/Components/BoxCollider2D.h"
#include "ECS/Components/LightComponent.h"
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
			if (ImGui::MenuItem("Light"))
			{
				m_SelectedEntity.AddComponent<LightComponent>({});
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	
	
	ImGui::End();
	
	ImGui::ShowDemoWindow();
}

void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
{
	m_SelectedEntity = entity;
}

Entity SceneHierarchyPanel::GetSelectedEntity()
{
	return m_SelectedEntity;
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
		std::cout << entity.GetHandle() << "\n";
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
		DrawComponent<LightComponent>("Light", entity, [](auto& component, Entity entity)
		{
			auto light = (LightComponent*)component;
			ImGui::ColorEdit3("Color", glm::value_ptr(light->ambient));
			ImGui::ColorEdit3("Specular", glm::value_ptr(light->specular));
			if (ImGui::DragFloat("Intensity", &(light->diffuse.x), 0.1f, 0))
			{
				light->diffuse = { light->diffuse.x,light->diffuse.x,light->diffuse.x };
			}
			const char* items[] = { "Point", "Spot", "Direction"};
			static int item_current_idx = 0; // Here we store our selection data as an index.
			switch (light->type)
			{
			case LightComponent::PointLight:
				item_current_idx = 0;
				break;
			case LightComponent::SpotLight:
				item_current_idx = 1;
				break;
			case LightComponent::DirectionLight:
				item_current_idx = 2;
				break;
			}
			const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)

			if (ImGui::BeginCombo("combo 1", combo_preview_value))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
					{
						item_current_idx = n;
						if (item_current_idx == 0)
						{
							light->type = LightComponent::PointLight;
						}
						else if (item_current_idx == 1)
						{
							light->type = LightComponent::SpotLight;
						}
						else if (item_current_idx == 2)
						{
							light->type = LightComponent::DirectionLight;
						}
					}
					

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (light->type == LightComponent::PointLight)
			{
				ImGui::DragFloat("constant", &(light->constant), 0.1f, 0, 10, "%.3f", ImGuiSliderFlags_Logarithmic);
				ImGui::DragFloat("linear", &(light->linear), 0.01f, 0,10,"%.3f", ImGuiSliderFlags_Logarithmic);
				ImGui::DragFloat("quadratic", &(light->quadratic), 0.001f, 0, 10, "%.3f", ImGuiSliderFlags_Logarithmic);
			}
			else if(light->type == LightComponent::SpotLight)
			{
				DrawVec3Control("direction", light->direction);
				ImGui::DragFloat("inner", &(light->innerCutOff), 0.1f, 0, 1,"%.3f", ImGuiSliderFlags_Logarithmic);
				ImGui::DragFloat("outer", &(light->outerCutoff), 0.1f, 0, 1);
			}
			else if (light->type == LightComponent::DirectionLight)
			{
				DrawVec3Control("direction", light->direction);
			}
		});
	}
}
