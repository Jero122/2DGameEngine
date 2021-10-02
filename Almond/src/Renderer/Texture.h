#pragma once
#include <cassert>
#include <string>
#include "GLCall.h"


class Texture
{
public:
	Texture(const std::string filePath);

	unsigned GetTexID() const
	{
		return m_TexID;
	}

	int GetWidth() const
	{
		return m_Width;
	}

	int GetHeight() const
	{
		return m_Height;
	}

private:
	unsigned int m_TexID;
	int m_Width;
	int m_Height;
	std::string m_FilePath;
};
