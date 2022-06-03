#pragma once
#include <GL/glew.h>

class GLTexture
{
public:
	GLTexture(GLenum type, int width, int height, GLenum internalFormat)
		:m_Type(type), m_Width(width),m_Height(height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(id, 1, internalFormat, m_Width, m_Height);
	}

	GLTexture(int width, int height, GLenum externalFormat, const void *image)
		:m_Type(GL_TEXTURE_2D), m_Width(width), m_Height(height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(id, 1, GL_RGB8, m_Width, m_Height);
		glTextureSubImage2D(id, 0, 0, 0, m_Width, m_Height, externalFormat, GL_UNSIGNED_BYTE, image);
	}


	GLTexture(const GLTexture&) = delete;
	GLTexture::GLTexture(GLTexture&& other)
		: m_Type(other.m_Type)
		, id(other.id)
	{
		other.m_Type = 0;
		other.id = 0;
	}

	GLTexture::~GLTexture()
	{
		//TODO manage deletion of textures in a resource manager
		//glDeleteTextures(1, &id);
	}

	int GetWidth() const
	{
		return m_Width;
	}
	int GetHeight() const
	{
		return m_Height;
	}

	int ID() const
	{
		return id;
	}

private:
	unsigned int id;
	GLenum m_Type;
	int m_Width;
	int m_Height;
};
