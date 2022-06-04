#pragma once
#include <cstddef>
#include <iostream>
#include <GL/glew.h>

#include "GLRenderBuffer.h"

class GLFrameBuffer
{
public:
	GLFrameBuffer(int m_width, int m_height)
		: m_Width(m_width),
		  m_Height(m_height)
	{
		//glGenFramebuffers(1, &id);
		glCreateFramebuffers(1, &id);
	}

	void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glViewport(0, 0, m_Width, m_Height);
	}

	void UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void AddColourAttachment()
	{
		/*glCreateTextures(GL_TEXTURE_2D, 1, &m_ColourAttachment);
		glTextureParameteri(m_ColourAttachment, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(m_ColourAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ColourAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(m_ColourAttachment, 1, GL_RGB8, m_Width, m_Height);
		glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0, m_ColourAttachment, 0);*/

		m_ColourAttachment = std::make_unique<GLTexture>(GL_TEXTURE_2D, m_Width, m_Height, GL_RGB8);
		glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0, m_ColourAttachment->ID(), 0);
	}

	void AddDepthAttachment()
	{
		m_DepthAttachment = std::make_unique<GLRenderBuffer>(GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment->ID());
	}

	void Invalidate(int width, int height)
	{
		if (id != 0)
		{
			glDeleteFramebuffers(1, &id);
			//glDeleteTextures(1, &m_ColourAttachment);
			//glDeleteRenderbuffers(1, &m_DepthAttachment);
		}

		m_Width = width;
		m_Height = height;
		glCreateFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		m_ColourAttachment->Resize(m_Width, m_Height);
		glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0, m_ColourAttachment->ID(), 0);

		AddDepthAttachment();

		if (glCheckNamedFramebufferStatus(id,GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
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

	std::unique_ptr<GLTexture> m_ColourAttachment;
	std::unique_ptr<GLRenderBuffer> m_DepthAttachment;
	//unsigned int m_ColourAttachment;
	//unsigned int m_DepthAttachment;
};
