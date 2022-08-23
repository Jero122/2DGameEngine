#include "TextureSystem.h"

#include "imgui.h"
#include "Core/Log.h"

void TextureSystem::OnStart()
{
}

void TextureSystem::OnEnd()
{
}

void TextureSystem::OnUpdate(TimeStep timeStep)
{
}

void TextureSystem::OnImGuiRender()
{
	ImGui::Begin("Texture System");
	ImGui::Text("Registered Textures");

	for (auto& texture : s_registeredTextures)
	{
		ImGui::Text("1");
	}

	ImGui::End();
}

void TextureSystem::OnLateUpdate()
{
}

std::shared_ptr<Texture> TextureSystem::Accquire(std::string filePath, bool autoRelease)
{
	auto find = s_registeredTextureTable.find(filePath);
	if (find == s_registeredTextureTable.end())
	{
		//Texture not found
		//create texture
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(filePath);
		TextureReference textureReference;
		textureReference.autoRelease = autoRelease;
		textureReference.referenceCount = 1;
		textureReference.index = 0;

		//Find an empty slot
		for (int i = 0; i < s_registeredTextures.size(); ++i)
		{
			if (s_registeredTextures[i] == nullptr)
			{
				//empty slot found,
				//Add to registered textures
				s_registeredTextures[i] = texture;
				//Add to lookup table
				textureReference.index = i;
				s_registeredTextureTable.insert(std::make_pair(filePath, textureReference));
				return s_registeredTextures[i];
			}
		}
	}
	else
	{
		//Texture found
		//Return the texture at appropriate index
		TextureReference& ref = find->second;
		ref.referenceCount++;

		return s_registeredTextures[ref.index];
	}
}

void TextureSystem::Release(std::string filePath)
{
	auto find = s_registeredTextureTable.find(filePath);
	//Texture not found
	if (find == s_registeredTextureTable.end())
	{
		AL_ENGINE_ERROR("Texture System: Trying to release a non existant texture: {0}", filePath);
		return;
	}
	else
	{
		TextureReference& ref = find->second;
		//Releasing a texture with 0 references
		if (ref.referenceCount == 0)
		{
			AL_ENGINE_ERROR("Texture System: Trying to release a non auto-release texture: {0}", filePath);
			return;
		}
		else
		{
			//Decrement reference
			ref.referenceCount--;
			//If reference reaches 0 and autorelease is true, release the texture
			if (ref.referenceCount == 0 && ref.autoRelease)
			{
				s_registeredTextures[ref.index] = nullptr;
				s_registeredTextureTable.erase(filePath);
			}
		}
	}

}
