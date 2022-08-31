#pragma once
#include <memory>
#include <unordered_map>

#include "Core/SubSystem.h"
#include "Renderer/Material.h"

struct MaterialConfig
{
	std::string name;
	bool auto_release;

	glm::vec4 albedoColour = { 0.0f,0.0f,0.0f, 0.0f };
	glm::vec4 emissiveColour = { 1.0f, 1.0f, 0.0f ,0.0f };

	float metallic = 0.0f;
	float roughness = 0.0f;

	float transparencyFactor = 1.0f;
	float alphaTest = 0.0f;

	std::string albedo_map_name;
	std::string ao_roughness_metallic_name;
	std::string normal_map_name;
	std::string emissive_map_name;
};

class MaterialSystem :
    public SubSystem
{
public:
	MaterialSystem(int maxMaterialCount)
		:m_MaxMaterialCount(maxMaterialCount)
	{
		s_registeredMaterials.reserve(m_MaxMaterialCount);
		s_registeredMaterialTable.reserve(maxMaterialCount);
	}

	~MaterialSystem() = default;

	void OnStart() override;
	void OnEnd() override;
	void OnUpdate(TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;

	/*
	 * Accquires a material from file
	 * @param name The name of the material file to load the material from, can look like "Assets/Models/RubberDuck/DuckMaterial.amt"
	 */
	static std::shared_ptr<Material> Accquire(std::string name);
	/*
	 * Accquires a material from code and serializes it as an .amt file
	 * @param materialConfig The material configuration used to generate and serialise a material
	 */
	static std::shared_ptr<Material> Accquire(MaterialConfig materialConfig);

	/*
	 * Releases a material by name
	 * @param name The name of the material to release
	 */
	static void Release(std::string name);

private:
	struct MaterialReference
	{
		int index = -1;
		int referenceCount = -1;
		bool autoRelease = false;
	};

	static bool loadMaterial(MaterialConfig config, std::shared_ptr<Material> material);
	static void destoryMaterial(std::shared_ptr<Material> material);
	bool load_configuration_file(std::string path, MaterialConfig& out_config);

	inline static std::vector<std::shared_ptr<Material>> s_registeredMaterials;
	inline static std::unordered_map<std::string, MaterialReference> s_registeredMaterialTable;

	int m_MaxMaterialCount = 0;

};