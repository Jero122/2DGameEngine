#include "GLCubeMap.h"

#include "Core/Log.h"
#include "Renderer/Bitmap.h"
#include "Renderer/UtilCubeMap.h"
#include "stb/stb_image.h"

GLCubeMap::GLCubeMap(const char* fileName)
{
	int w, h, comp;
	stbi_set_flip_vertically_on_load(false);
	const float* img = stbi_loadf(fileName, &w, &h, &comp, 3);
	Bitmap in(w, h, comp, eBitmapFormat_Float, img);
	Bitmap out = convertEquirectangularMapToVerticalCross(in);
	stbi_image_free((void*)img);
	stbi_set_flip_vertically_on_load(true);
	Bitmap cubemap = convertVerticalCrossToCubeMapFaces(out);

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &id);
	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(id, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(id, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(id, 1, GL_RGB32F, cubemap.w_, cubemap.h_);
	const uint8_t* data = cubemap.data_.data();

	for (unsigned i = 0; i != 6; ++i)
	{
		glTextureSubImage3D(id, 0, 0, 0, i, cubemap.w_, cubemap.h_, 1, GL_RGB, GL_FLOAT, data);
		data += cubemap.w_ * cubemap.h_ * cubemap.comp_ * Bitmap::getBytesPerComponent(cubemap.fmt_);
	}

	AL_ENGINE_INFO("Cubemap loaded at path:{0}", fileName);

	glGenerateTextureMipmap(id);
}

GLCubeMap::GLCubeMap(std::vector<std::string> faces)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		stbi_set_flip_vertically_on_load(true);
		if (data)
		{
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

			AL_ENGINE_INFO("Cubemap texture loaded at path:{0}", faces[i]);
		}
		else
		{
			AL_ENGINE_ERROR("Cubemap texture failed to load at path: {0}", faces[i]);
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

GLCubeMap::~GLCubeMap()
{
	//TODO manage deletion of textures in a resource manager
	if (id != 0)
	{
		glDeleteTextures(1, &id);
	}
}

GLCubeMap::GLCubeMap(GLCubeMap&& other)
	:id(other.id)
	, m_Width(other.m_Width)
	, m_Height(other.m_Height)
{
	other.id = 0;
	other.m_Width = 0;
	other.m_Height = 0;
}


int GLCubeMap::GetWidth() const
{
	return m_Width;
}

int GLCubeMap::GetHeight() const
{
	return m_Height;
}

int GLCubeMap::ID() const
{
	return id;
}
