#include "Texture.h"

#include <iostream>

#include "stb/stb_image.h"

Texture::Texture(const std::string filePath)
	:m_FilePath(filePath)
{
	int nrComponents;
	unsigned char* data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		/*glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(id, 1, GL_RGB8, m_Width, m_Height);
		glTextureSubImage2D(id, 0, 0, 0, m_Width, m_Height, format, GL_UNSIGNED_BYTE, data);*/
		
		m_Texture = std::make_shared<GLTexture>(m_Width, m_Height, format, data);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filePath << std::endl;
		stbi_image_free(data);
	}
}
