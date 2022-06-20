#pragma once
#include <GL/glew.h>
class GLRenderBuffer
{
public:
	GLRenderBuffer(GLenum internalFormat, int width, int height)
		:m_InternalFormat(internalFormat), m_Width(width), m_Height(height)
	{
		glCreateRenderbuffers(1, &id);
		glNamedRenderbufferStorage(id, m_InternalFormat, m_Width, m_Height);
	}

	~GLRenderBuffer()
	{
		if (id != 0)
		{
			glDeleteRenderbuffers(1, &id);
		}
	}
	unsigned int ID()
	{
		return id;
	}

	GLenum GetInternalFormat()
	{
		return m_InternalFormat;
	}

private:
	unsigned int id;
	GLenum m_InternalFormat;
	int m_Width;
	int m_Height;
};
