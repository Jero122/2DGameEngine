#pragma once
#include <GL/glew.h>
#include<iostream>
#include <vector>

#include "GLTexture.h"
#include "GLRenderBuffer.h"
#include "Core/Log.h"

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

	void AddColourAttachment(GLenum internalFormat, GLint filterMode)
	{
		m_ColourAttachments.push_back(std::make_unique<GLTexture>(GL_TEXTURE_2D, m_Width, m_Height, internalFormat));
		auto& attachment = m_ColourAttachments[ColourAttachmentIdx];
		glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + ColourAttachmentIdx, attachment->ID(), 0);
		m_AttachmentTargets.push_back(GL_COLOR_ATTACHMENT0 + ColourAttachmentIdx);
		ColourAttachmentIdx += 1;
	}

	void AddDepthAttachment()
	{
		m_DepthAttachment = std::make_unique<GLRenderBuffer>(GL_DEPTH24_STENCIL8, m_Width, m_Height);
		glNamedFramebufferRenderbuffer(id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment->ID());
	}

	int ReadColourAttachment(int attachmentIdx, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIdx);

		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void ClearColourAttachment(int attachmentIdx, int value)
	{
		glClearTexImage(m_ColourAttachments[attachmentIdx]->ID(), 0, GL_RED_INTEGER ,GL_INT, &value);
	}

	void Invalidate(int width, int height)
	{
		if (id != 0)
		{
			glDeleteFramebuffers(1, &id);
		}

		m_Width = width;
		m_Height = height;
		glCreateFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		for (int i = 0; i < ColourAttachmentIdx; ++i)
		{
			auto& attachment = m_ColourAttachments[i];
			attachment->Resize(m_Width, m_Height);
			glNamedFramebufferTexture(id, GL_COLOR_ATTACHMENT0 + i, attachment->ID(), 0);
		}

		AddDepthAttachment();

		if (m_ColourAttachments.size() > 1)
		{
			glDrawBuffers(m_ColourAttachments.size(), m_AttachmentTargets.data());
		}
		else if (m_ColourAttachments.empty())
		{
			// Only depth-pass
			//glDrawBuffer(GL_NONE);
		}
		if (glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			AL_ENGINE_ERROR("Frambuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	unsigned int GetAttachmentID(unsigned int index)
	{
		return m_ColourAttachments[index]->ID();
	}

	unsigned int GetDepthAttachmentID()
	{
		return m_DepthAttachment->ID();
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

	std::vector<std::unique_ptr<GLTexture>> m_ColourAttachments;
	std::vector<unsigned int> m_AttachmentTargets;
	unsigned int ColourAttachmentIdx = 0;

	//std::unique_ptr<GLTexture> m_ColourAttachment;
	std::unique_ptr<GLRenderBuffer> m_DepthAttachment;
	//unsigned int m_ColourAttachment;
	//unsigned int m_DepthAttachment;
};
