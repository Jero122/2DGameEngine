#include "GLTexture.h"
#include <cassert>
#include "stb/stb_image.h"

int getNumMipMapLevels2D(int w, int h)
{
	int levels = 1;
	while ((w | h) >> levels)
		levels += 1;
	return levels;
}

/// Draw a checkerboard on a pre-allocated square RGB image.
uint8_t* genDefaultCheckerboardImage(int* width, int* height)
{
	const int w = 128;
	const int h = 128;

	uint8_t* imgData = (uint8_t*)malloc(w * h * 3); // stbi_load() uses malloc(), so this is safe

	assert(imgData && w > 0 && h > 0);
	assert(w == h);

	if (!imgData || w <= 0 || h <= 0) return nullptr;
	if (w != h) return nullptr;

	for (int i = 0; i < w * h; i++)
	{
		const int row = i / w;
		const int col = i % w;
		imgData[i * 3 + 0] = imgData[i * 3 + 1] = imgData[i * 3 + 2] = 0xFF * ((row + col) % 2);
	}

	if (width) *width = w;
	if (height) *height = h;

	return imgData;
}

GLTexture::GLTexture(GLenum type, const char* fileName)
	:GLTexture(type, fileName, GL_REPEAT)
{
}

GLTexture::GLTexture(GLenum type, const char* fileName, GLenum clamp)
	:m_Type(type)
{
	switch (type)
	{
	case GL_TEXTURE_2D:
	{
		glCreateTextures(type, 1, &id);
		glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, clamp);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, clamp);

		int numMipmaps = 0;

		uint8_t* img = stbi_load(fileName, &m_Width, &m_Height, nullptr, STBI_rgb_alpha);

		// If file is not found, a fallback texture is used
		if (!img)
		{
			fprintf(stderr, "WARNING: could not load image `%s`, using a fallback.\n", fileName);
			img = genDefaultCheckerboardImage(&m_Width, &m_Height);
			if (!img)
			{
				fprintf(stderr, "FATAL ERROR: out of memory allocating image for fallback texture\n");
				exit(EXIT_FAILURE);
			}
		}

		numMipmaps = getNumMipMapLevels2D(m_Width, m_Height);
		glTextureStorage2D(id, numMipmaps, GL_RGBA8, m_Width, m_Height);
		glTextureSubImage2D(id, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, img);
		stbi_image_free((void*)img);
		break;
	}
	default:
		assert(false);
	}

}

GLTexture::GLTexture(GLenum type, int width, int height, GLenum internalFormat)
	: m_Type(type), m_Width(width), m_Height(height), m_InternalFormat(internalFormat)
{
	glCreateTextures(type, 1, &id);
	glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(id, getNumMipMapLevels2D(width, height), internalFormat, width, height);
}

GLTexture::GLTexture(int width, int height, GLenum externalFormat, const void* image)
	:m_Type(GL_TEXTURE_2D), m_Width(width), m_Height(height)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(id, 1, GL_RGBA8, m_Width, m_Height);
	glTextureSubImage2D(id, 0, 0, 0, m_Width, m_Height, externalFormat, GL_UNSIGNED_BYTE, image);
}

void GLTexture::Resize(int width, int height)
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

void GLTexture::SetFilterMode(GLint filterMode)
{
	m_FilterMode = filterMode;
	glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, m_FilterMode);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, m_FilterMode);
}

int GLTexture::GetWidth() const
{
	return m_Width;
}

int GLTexture::GetHeight() const
{
	return m_Height;
}

GLenum GLTexture::GetInternalFormat()
{
	return m_InternalFormat;
}

int GLTexture::ID() const
{
	return id;
}

GLTexture::GLTexture(GLTexture&& other)
	: m_Type(other.m_Type)
	, id(other.id)
	, m_InternalFormat(other.m_InternalFormat)
	, m_FilterMode(other.m_FilterMode)
	, m_Width(other.m_Width)
	, m_Height(other.m_Height)
{
	other.m_Type = 0;
	other.id = 0;
	other.m_InternalFormat = 0;
	other.m_FilterMode = 0;
	other.m_Width = 0;
	other.m_Height = 0;
}

GLTexture::~GLTexture()
{
	//TODO manage deletion of textures in a resource manager
	if (id != 0)
	{
		glDeleteTextures(1, &id);
	}
}
