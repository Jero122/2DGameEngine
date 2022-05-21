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
		return id;
	}

	int GetWidth() const
	{
		return m_Width;
	}

	int GetHeight() const
	{
		return m_Height;
	}


	std::string Type()
	{
		return type;
	}

	std::string FilePath()
	{
		return m_FilePath;
	}

	unsigned int id;
private:

	int m_Width;
	int m_Height;
	std::string type;
	std::string m_FilePath;

};
