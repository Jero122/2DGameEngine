#include "SceneSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include "ECS/SceneView.h"
#include "ECS/Entity.h"
#include "ECS/Components/BoxCollider2D.h"
#include "ECS/Components/RigidBody.h"
#include "ECS/Components/SpriteRenderer.h"
#include "ECS/Components/TagComponent.h"
#include "ECS/Components/Transform.h"

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& vec)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
	return out;
}

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


static std::string RigidBodyTypeToString(RigidBody::BodyType bodyType)
{
	switch (bodyType)
	{
		case RigidBody::BodyType::Dynamic: return "Dynamic";
		case RigidBody::BodyType::Static: return "Static";
		case RigidBody::BodyType::Kinematic: return "Kinematic";
	}
}

static RigidBody::BodyType RigidBodyTypeFromString(std::string bodyType)
{
	if (bodyType == "Dynamic")
	{
		return RigidBody::BodyType::Dynamic;
	}
	else if (bodyType == "Static")
	{
		return RigidBody::BodyType::Static;
	}
	else if (bodyType == "Kinematic")
	{
		return RigidBody::BodyType::Kinematic;
	}
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
	if (entity.HasComponent<RigidBody>())
	{
		out << YAML::Key << "Rigid Body";
		out << YAML::BeginMap; // RigidBody

		auto rb = entity.GetComponent<RigidBody>();

		out << YAML::Key << "Body Type" << YAML::Value << RigidBodyTypeToString(rb->Type);
		out << YAML::Key << "Fixed Rotation" << YAML::Value << rb->FixedRotation;

		out << YAML::EndMap; // RigidBody
	}
	if (entity.HasComponent<BoxCollider2D>())
	{
		out << YAML::Key << "Box Collider2D";
		out << YAML::BeginMap; // BoxCollider2D

		auto collider = entity.GetComponent<BoxCollider2D>();

		out << YAML::Key << "Offset" << YAML::Value << collider->Offset;
		out << YAML::Key << "Size" << YAML::Value << collider->Size;
		out << YAML::Key << "Density" << YAML::Value << collider->Density;
		out << YAML::Key << "Friction" << YAML::Value << collider->Friction;
		out << YAML::Key << "Restitution" << YAML::Value << collider->Restitution;
		out << YAML::Key << "Restitution Threshold" << YAML::Value << collider->RestitutionThreshold;

		out << YAML::EndMap; // BoxCollider2D
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
