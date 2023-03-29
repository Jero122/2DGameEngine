#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "Core/SubSystem.h"
#include "Renderer/Texture.h"

class TextureSystem :
    public SubSystem
{
public:
	TextureSystem(int maxTextureCount)
		:m_MaxTextureCount(maxTextureCount)
	{
		s_registeredTextures.resize(m_MaxTextureCount);
		s_registeredTextureTable.reserve(m_MaxTextureCount);
	}

	~TextureSystem();

	/**
	 * \brief Returns a default 128x128 white texture
	 * \return 
	 */
	static std::shared_ptr<Texture> AccquireDefaultAlbedo();
	/**
	 * \brief Returns a default 128x128 black AO/MetallicRoughness texture
	 * \return 
	 */
	static std::shared_ptr<Texture> AccquireDefaultAOMetallicRoughness();
	/**
	 * \brief Returns a default 128x128 black emissive texture
	 * \return 
	 */
	static std::shared_ptr<Texture> AccquireDefaultEmissive();
	/**
	 * \brief Returns a default 128x128 Flat (#8080FF) normal texture
	 * \return 
	 */
	static std::shared_ptr<Texture> AccquireDefaulNormal();

	/**
	 * \brief Acquires a texture from file 
	 * \param filePath 
	 * \param autoRelease whether to release this texture automatically upon 0 references
	 * \return shared pointer to the Texture asset
	 */
	static std::shared_ptr<Texture> Acquire(std::string filePath, bool autoRelease);

	/**
	 * \brief Releases this texture by filePath (name)
	 * \param filePath 
	 */
	static void Release(std::string filePath);
private:
	struct TextureReference
	{
		int index = -1;
		int referenceCount = -1;
		bool autoRelease = false;
	};

	static std::shared_ptr<Texture> AccquireDefaultTexture(const char* name, uint8_t r, uint8_t g, uint8_t b);
	inline static std::vector<std::shared_ptr<Texture>> s_registeredTextures;
	inline static std::unordered_map<std::string, TextureReference> s_registeredTextureTable;

	int m_MaxTextureCount = 0;
};

