#pragma once
#include <cstdint>

class OpenGLVertexBuffer
{
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	virtual ~OpenGLVertexBuffer(){}

	void bind() const;
	void unbind() const;

private:
	unsigned int ID;

};

class OpenGLIndexBuffer
{
public:
	OpenGLIndexBuffer(const void* indices, uint32_t size);
	virtual ~OpenGLIndexBuffer() {}

	void bind() const;
	void unbind() const;

private:
	unsigned int ID;
};
