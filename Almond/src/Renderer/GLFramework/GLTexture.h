#pragma once
#include <GL/glew.h>
#include <iostream>

class GLTexture
{
public:
	GLTexture(GLenum type,const char* fileName);
	GLTexture(GLenum type, const char* fileName, GLenum clamp);
	GLTexture(GLenum type, int width, int height, GLenum internalFormat);
	GLTexture(int width, int height, GLenum externalFormat, const uint8_t* image);

	~GLTexture();

	void Resize(int width, int height);
	void SetFilterMode(GLint filterMode);

	GLTexture(const GLTexture&) = delete;
	GLTexture(GLTexture&&);

	int GetWidth() const;
	int GetHeight() const;
	GLenum GetInternalFormat();
	int ID() const;
	unsigned int id;

private:

	GLenum m_Type;
	GLenum m_InternalFormat;
	GLint m_FilterMode = GL_LINEAR;
	int m_Width;
	int m_Height;
};
