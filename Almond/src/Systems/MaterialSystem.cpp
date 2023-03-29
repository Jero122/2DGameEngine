#include "MaterialSystem.h"

#include "TextureSystem.h"
#include "Core/Log.h"


std::shared_ptr<Material> MaterialSystem::Accquire(std::string name)
{
	return nullptr;
}

std::shared_ptr<Material> MaterialSystem::Accquire(MaterialConfig materialConfig)
{
	auto find = s_registeredMaterialTable.find(materialConfig.name);

	if (find == s_registeredMaterialTable.end())
	{
		//Material not found
		//Create Material
		std::shared_ptr<Material> material = std::make_shared<Material>();
		if (!loadMaterial(materialConfig, material))
		{
			AL_ENGINE_ERROR("Material System: Failed to load material: {0}", materialConfig.name);
			return nullptr;
		}
		
		MaterialReference materialReference;
		materialReference.autoRelease = materialConfig.auto_release;
		materialReference.referenceCount = 1;

		//Find an empty slot in registered materials array
		for (int i = 0; i < s_registeredMaterials.size(); ++i)
		{
			if (s_registeredMaterials[i] == nullptr)
			{
				//empty slot found, add material
				s_registeredMaterials[i] = material;
				//Add to lookup table
				materialReference.index = i;
				s_registeredMaterialTable.insert(std::make_pair(materialConfig.name, materialReference));
			}
		}
		return material;
	}
	
	//Material found, Increment ref count and return material
	MaterialReference& ref = find->second;
	ref.referenceCount++;
	return s_registeredMaterials[ref.index];
}

void MaterialSystem::Release(std::string name)
{
	auto find = s_registeredMaterialTable.find(name);
	if (find == s_registeredMaterialTable.end())
	{
		//Material not found
		AL_ENGINE_ERROR("Material System: Trying to release a non existant material: {0}", name);
	}
	else
	{
		MaterialReference& ref = find->second;
		//If reference count is 0, it would have to be a non auto-release material
		if (ref.referenceCount == 0)
		{
			AL_ENGINE_WARN("Material System: Trying to rlease a non auto-release material", name);
		}
		else
		{
			//decrement ref count
			ref.referenceCount--;
			//release if ref == 0
			if (ref.referenceCount == 0 && ref.autoRelease)
			{
				//destroy the material, releases textures etc
				destoryMaterial(s_registeredMaterials[ref.index]);
				//remove the material and its reference from array and lookup table
				s_registeredMaterials[ref.index] = nullptr;
				s_registeredMaterialTable.erase(name);
			}
		}
	}
}

bool MaterialSystem::loadMaterial(MaterialConfig config, std::shared_ptr<Material> material)
{
	//Acquire appropriate textures

	//albedo colour
	material->albedoColour = config.albedoColour;
	//emissive colour
	material->emissiveColour = config.emissiveColour;
	//metallic
	material->metallic = config.metallic;
	//roughness
	material->roughness = config.roughness;
	//transparency
	material->transparencyFactor = config.transparencyFactor;
	//alphaTest
	material->alphaTest = config.alphaTest;

	//Acquire relevant textures from the Texture System
	if (config.albedo_map_name.empty())
	{
		material->albedoMap = TextureSystem::AccquireDefaultAlbedo();
	}
	else
	{
		material->albedoMap = TextureSystem::Acquire(config.albedo_map_name, true);
	}

	if (config.ao_roughness_metallic_name.empty())
	{
		material->aoRoughnessMetallicMap = TextureSystem::AccquireDefaultAOMetallicRoughness();
	}
	else
	{
		material->aoRoughnessMetallicMap = TextureSystem::Acquire(config.ao_roughness_metallic_name, true);
	}

	if (config.normal_map_name.empty())
	{
		
		material->normalMap = TextureSystem::AccquireDefaulNormal();
	
	}
	else
	{
		material->normalMap = TextureSystem::Acquire(config.normal_map_name, true);
	}

	if (config.emissive_map_name.empty())
	{
		material->emissiveMap = TextureSystem::AccquireDefaultEmissive();
	}
	else
	{
		material->emissiveMap = TextureSystem::Acquire(config.emissive_map_name, true);
	}

	return true;
}

void MaterialSystem::destoryMaterial(std::shared_ptr<Material> material)
{
	//Release appropriate textures

	if (material->albedoMap)
	{
		TextureSystem::Release(material->albedoMap->m_FilePath);
	}

	if (material->aoRoughnessMetallicMap)
	{
		TextureSystem::Release(material->aoRoughnessMetallicMap->m_FilePath);
	}

	if (material->normalMap)
	{
		TextureSystem::Release(material->normalMap->m_FilePath);
	}

	if (material->emissiveMap)
	{
		TextureSystem::Release(material->emissiveMap->m_FilePath);
	}

	//reset memory
	material = nullptr;
}

bool MaterialSystem::load_configuration_file(std::string path, MaterialConfig& out_config)
{
	return false;
}
