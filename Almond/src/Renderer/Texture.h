#pragma once
#include <memory>
#include <string>

#include "GLFramework/GLTexture.h"

class Texture
{
public:
	Texture(const std::string filePath);

	int GetWidth() const
	{
		return m_Width;
	}

	int GetHeight() const
	{
		return m_Height;
	}

	unsigned int ID() const
	{
		return m_Texture->ID();
	}

	std::string m_FilePath;
	std::string type;
private:
	std::shared_ptr<GLTexture> m_Texture;
	int m_Width;
	int m_Height;
};
