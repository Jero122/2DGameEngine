#include "TextureSystem.h"

#include "imgui.h"
#include "Core/Log.h"
#include "stb/stb_image.h"

std::shared_ptr<Texture> TextureSystem::AccquireDefaultAlbedo()
{
	auto name = "DefaultAlbedo";
	uint8_t r = 0xFF;
	uint8_t g = 0xFF;
	uint8_t b = 0xFF;

	std::shared_ptr<Texture> texture = AccquireDefaultTexture(name, r, g, b);
	return texture;
}

std::shared_ptr<Texture> TextureSystem::AccquireDefaultAOMetallicRoughness()
{
	auto name = "DefaultAOMetallicRoughness";
	uint8_t r = 0x00;
	uint8_t g = 0x00;
	uint8_t b = 0x00;

	std::shared_ptr<Texture> texture = AccquireDefaultTexture(name, r, g, b);
	return texture;
}

std::shared_ptr<Texture> TextureSystem::AccquireDefaultEmissive()
{
	auto name = "DefaultEmissive";
	uint8_t r = 0x00;
	uint8_t g = 0x00;
	uint8_t b = 0x00;

	std::shared_ptr<Texture> texture = AccquireDefaultTexture(name, r, g, b);

	return texture;
}

std::shared_ptr<Texture> TextureSystem::AccquireDefaulNormal()
{
	auto name = "DefaultNormal";
	uint8_t r = 0x80;
	uint8_t g = 0x80;
	uint8_t b = 0xFF;

	std::shared_ptr<Texture> texture = AccquireDefaultTexture(name, r, g, b);

	return texture;
}

std::shared_ptr<Texture> TextureSystem::AccquireDefaultTexture(const char* name, uint8_t r, uint8_t g, uint8_t b)
{
	auto find = s_registeredTextureTable.find(name);
	if (find == s_registeredTextureTable.end())
	{
		//Texture not found
		//create default texture
		const int w = 128;
		const int h = 128;
		uint8_t* imgData = (uint8_t*)malloc(w * h * 3); // stbi_load() uses malloc(), so this is safe

		for (int i = 0; i < w * h; i++)
		{
			const int row = i / w;
			const int col = i % w;
			imgData[i * 3 + 0] = r;
			imgData[i * 3 + 1] = g;
			imgData[i * 3 + 2] = b;
		}

		std::shared_ptr<Texture> texture = std::make_shared<Texture>(w, h, GL_RGB, imgData);

		//Free the imgData
		stbi_image_free((void*)imgData);

		TextureReference textureReference;
		textureReference.autoRelease = false;
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
				s_registeredTextureTable.insert(std::make_pair(name, textureReference));
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
	return nullptr;
}

std::shared_ptr<Texture> TextureSystem::Acquire(std::string filePath, bool autoRelease)
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

	//TODO handle edge case?
	return nullptr;
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
			AL_ENGINE_WARN("Texture System: Trying to release a non auto-release texture: {0}", filePath);
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

TextureSystem::~TextureSystem()
{
	//remove all textures from the registered texture table
	for (auto & pair : s_registeredTextureTable)
	{
		auto& reference = pair.second;
		s_registeredTextures[reference.index] = nullptr;
	}

	//Remove all entries from the texture lookup table
	s_registeredTextureTable.erase(s_registeredTextureTable.begin(), s_registeredTextureTable.end());
}
