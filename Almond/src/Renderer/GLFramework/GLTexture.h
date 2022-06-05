#pragma once
#include <GL/glew.h>
#include <iostream>

class GLTexture
{
public:
	GLTexture(GLenum type, int width, int height, GLenum internalFormat, GLint filterMode)
		:m_Type(type), m_Width(width),m_Height(height), m_InternalFormat(internalFormat), m_FilterMode(filterMode)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, filterMode);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, filterMode);
		glTextureStorage2D(id, 1, m_InternalFormat, m_Width, m_Height);
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


	GLTexture::~GLTexture()
	{
		//TODO manage deletion of textures in a resource manager
		if (id !=0)
		{
			glDeleteTextures(1, &id);
		}
	}

	void Resize(int width, int height)
	{
		if (id != 0)
		{
			glDeleteTextures(1, &id);
		}

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, m_FilterMode);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, m_FilterMode);
		glTextureStorage2D(id, 1, m_InternalFormat, m_Width, m_Height);
	}

	void SetFilterMode(GLint filterMode)
	{
		m_FilterMode = filterMode;
		glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, m_FilterMode);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, m_FilterMode);
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
	GLenum m_InternalFormat;
	GLint m_FilterMode = GL_LINEAR;
	int m_Width;
	int m_Height;
};
