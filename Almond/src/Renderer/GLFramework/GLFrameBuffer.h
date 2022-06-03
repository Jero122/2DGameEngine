#pragma once
#include <cstddef>
#include <iostream>
#include <GL/glew.h>

class GLFrameBuffer
{
public:
	GLFrameBuffer(int m_width, int m_height)
		: m_Width(m_width),
		  m_Height(m_height)
	{
		glGenFramebuffers(1, &id);
	}

	void Bind()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
	}

	void UnBind()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void AddColourAttachment()
	{
		glGenTextures(1, &m_ColourAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColourAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourAttachment, 0);
	}

	void AddDepthAttachment()
	{
		glGenRenderbuffers(1, &m_DepthAttachment);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment); // now actually attach it
	}

	void Invalidate(int width, int height)
	{
		if (id != 0)
		{
			glDeleteFramebuffers(1, &id);
			glDeleteTextures(1, &m_ColourAttachment);
			glDeleteRenderbuffers(1, &m_DepthAttachment);
		}

		m_Width = width;
		m_Height = height;
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
		AddColourAttachment();
		AddDepthAttachment();

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	unsigned int ID()
	{
		return  id;
	}

	int GetWidth()
	{
		return m_Width;
	}

	int GetHeight()
	{
		return m_Height;
	}
private:
	unsigned int id;
	int m_Width;
	int m_Height;

	unsigned int m_ColourAttachment;
	unsigned int m_DepthAttachment;
};
