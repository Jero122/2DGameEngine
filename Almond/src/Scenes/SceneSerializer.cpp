#include "SceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include "ECS/SceneView.h"
#include "ECS/Entity.h"
#include "ECS/Components/SpriteRenderer.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/Transform.h"

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
	return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
	return out;
}


static void SerializeEntity(YAML::Emitter& out, Entity entity)
{
	out << YAML::BeginMap; // Entity
	out << YAML::Key << "Entity" << YAML::Value << entity.GetHandle();

	if (entity.HasComponent<TagComponent>())
	{
		out << YAML::Key << "Tag Component";
		out << YAML::BeginMap; // TagComponent

		auto& tag = entity.GetComponent<TagComponent>()->tag;
		out << YAML::Key << "Tag" << YAML::Value << tag;

		out << YAML::EndMap; // TagComponent
	}

	if (entity.HasComponent<Transform>())
	{
		out << YAML::Key << "Transform";
		out << YAML::BeginMap; // Transform

		auto transform = entity.GetComponent<Transform>();

		out << YAML::Key << "Position" << YAML::Value << transform->position;
		out << YAML::Key << "Rotation" << YAML::Value << transform->rotation;
		out << YAML::Key << "Scale" << YAML::Value << transform->scale;

		out << YAML::EndMap; // Transform
	}

	if (entity.HasComponent<SpriteRenderer>())
	{
		out << YAML::Key << "Sprite Renderer";
		out << YAML::BeginMap; // Sprite Renderer

		auto spriteRenderer = entity.GetComponent<SpriteRenderer>();

		out << YAML::Key << "Width" << YAML::Value << spriteRenderer->width;
		out << YAML::Key << "Height" << YAML::Value << spriteRenderer->height;
		out << YAML::Key << "Color" << YAML::Value << spriteRenderer->color;

		out << YAML::EndMap; // Sprite Renderer
	}



	out << YAML::EndMap; // Entity
}

void SceneSerializer::Serialize(const std::string& filepath)
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

	for (EntityID entityHandle : SceneView<>(m_Scene->m_Ecs))
	{
		Entity entity = Entity{ entityHandle, m_Scene.get() };
		SerializeEntity(out, entity);
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(filepath);
	fout << out.c_str();
}

void SceneSerializer::SerializeRuntime(const std::string& filepath)
{
}

void SceneSerializer::Deserialize(const std::string& filepath)
{
}

void SceneSerializer::DeserializeRuntime(const std::string& filepath)
{
}
