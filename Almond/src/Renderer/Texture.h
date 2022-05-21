#pragma once
#include <string>
#include "GLCall.h"

class Texture
{
public:

	Texture() = default;

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


	std::string m_FilePath;
	unsigned int id;
	std::string type;
private:

	int m_Width;
	int m_Height;


};
