#pragma once
#include "Scene.h"

class SceneSerializer
{
public:
	explicit SceneSerializer(const std::shared_ptr<Scene>& m_scene)
		: m_Scene(m_scene)
	{
	}

	void Serialize(const std::string& filepath);
	void SerializeRuntime(const std::string& filepath);

	void Deserialize(const std::string& filepath);
	void DeserializeRuntime(const std::string& filepath);

private:
	std::shared_ptr<Scene> m_Scene;
	
};
