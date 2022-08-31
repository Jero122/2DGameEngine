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

	void OnStart() override;
	void OnEnd() override;
	void OnUpdate(TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;

	static std::shared_ptr<Texture> Accquire(std::string filePath, bool autoRelease);
	static void Release(std::string filePath);
private:
	struct TextureReference
	{
		int index = -1;
		int referenceCount = -1;
		bool autoRelease = false;
	};

	inline static std::vector<std::shared_ptr<Texture>> s_registeredTextures;
	inline static std::unordered_map<std::string, TextureReference> s_registeredTextureTable;

	int m_MaxTextureCount = 0;
};

