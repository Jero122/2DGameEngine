#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <GL/glew.h>

enum class BufferAttribType
{
	None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Bool, Mat3, Mat4, UnsignedByte
};

static uint32_t AttribTypeSize(BufferAttribType type)
{
	switch (type)
	{
		case BufferAttribType::Float:		return 4;
		case BufferAttribType::Float2:	return 4 * 2;
		case BufferAttribType::Float3:	return 4 * 3;
		case BufferAttribType::Float4:	return 4 * 4;
		case BufferAttribType::Int:		return 4;
		case BufferAttribType::Int2:		return 4 * 2;
		case BufferAttribType::Int3:		return 4 * 3;
		case BufferAttribType::Int4:		return 4 * 4;
		case BufferAttribType::Bool:		return 1;
		case BufferAttribType::UnsignedByte:		return 4;
		case BufferAttribType::Mat3:		return 4 * 3 * 3;
		case BufferAttribType::Mat4:		return 4 * 4 * 4;
	}
	return 0;
}

struct BufferAttribute
{
	std::string Name;
	BufferAttribType Type;
	uint32_t Size;
	uint32_t Offset;
	uint32_t Count;
	bool Normalized;

	BufferAttribute(const std::string& name, BufferAttribType type, bool normalized)
		:Name(name), Type(type), Size(AttribTypeSize(type)), Offset(0), Normalized(normalized)
	{
		switch (type)
		{
			case BufferAttribType::Float:		Count = 1; break;
			case BufferAttribType::Float2:		Count = 2; break;
			case BufferAttribType::Float3:		Count = 3; break;
			case BufferAttribType::Float4:		Count = 4; break;
			case BufferAttribType::Int:			Count = 1; break;
			case BufferAttribType::Int2:		Count = 2; break;
			case BufferAttribType::Int3:		Count = 3; break;
			case BufferAttribType::Int4:		Count = 4; break;
			case BufferAttribType::Bool:		Count = 1; break;
			case BufferAttribType::UnsignedByte:		Count = 4; break;
			case BufferAttribType::Mat3:		Count = 3*3; break;
			case BufferAttribType::Mat4:		Count = 4*4; break;
			case BufferAttribType::None:		Count = 0; break;
		}
	}

	GLenum TypeToOpenGLType()
	{
		switch (Type)
		{
			case BufferAttribType::Float:		return GL_FLOAT;
			case BufferAttribType::Float2:	return GL_FLOAT;
			case BufferAttribType::Float3:	return GL_FLOAT;
			case BufferAttribType::Float4:	return GL_FLOAT;
			case BufferAttribType::Int:		return GL_INT;
			case BufferAttribType::Int2:		return GL_INT;
			case BufferAttribType::Int3:		return GL_INT;
			case BufferAttribType::Int4:		return GL_INT;
			case BufferAttribType::Bool:		return GL_BOOL;
			case BufferAttribType::UnsignedByte:		return GL_UNSIGNED_BYTE;
			case BufferAttribType::Mat3:		return GL_FLOAT;
			case BufferAttribType::Mat4:		return GL_FLOAT;
		}
		return 0;
	}
};

class BufferLayout
{
public:
	void AddAttribute(BufferAttribute attribute)
	{
		attribute.Offset = m_tempOffset;
		m_tempOffset += attribute.Size;
		m_Stride += attribute.Size;

		m_Attributes.push_back(attribute);
	}

	uint32_t GetStride() { return  m_Stride; }

	std::vector<BufferAttribute>::iterator begin() { return  m_Attributes.begin(); }
	std::vector<BufferAttribute>::iterator end() { return  m_Attributes.end(); }
private:
	std::vector<BufferAttribute> m_Attributes;
	uint32_t m_Stride = 0;
	uint32_t m_tempOffset = 0;
};

class OpenGLVertexBuffer
{
public:
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	virtual ~OpenGLVertexBuffer(){}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SetLayout(const BufferLayout& bufferLayout)
	{
		int index = 0;
		m_BufferLayout = bufferLayout;
		for (auto attribute : m_BufferLayout)
		{
			glVertexAttribPointer(index, attribute.Count, attribute.TypeToOpenGLType(), attribute.Normalized ? GL_TRUE : GL_FALSE, m_BufferLayout.GetStride(), (const void*)attribute.Offset);
			glEnableVertexAttribArray(index);
			index++;
		}
	}

	BufferLayout GetLayout()
	{
		return m_BufferLayout;
	}

private:
	BufferLayout m_BufferLayout;
	unsigned int ID;

};

class OpenGLIndexBuffer
{
public:

	OpenGLIndexBuffer::OpenGLIndexBuffer(const void* indices, uint32_t size)
	{
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	virtual ~OpenGLIndexBuffer() {}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
	}

private:
	unsigned int ID;
};
