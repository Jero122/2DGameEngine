#include "GLCubeMap.h"

#include "GLBuffer.h"
#include "GLFrameBuffer.h"
#include "GLVertexArray.h"
#include "Core/Log.h"
#include "Renderer/Bitmap.h"
#include "Renderer/Shader.h"
#include "Renderer/UtilCubeMap.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

GLCubeMap::GLCubeMap(const char* fileName)
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	//Empty Cube VAO
	//Cube vertices and indices are handled in the vertex shader using gl_VertexID
	auto cubeVAO = GLVertexArray();
	cubeVAO.Bind();

	AL_ENGINE_TRACE("Loading Cubemap: {0}", fileName);

	//Load hdr file
	int w, h, comp;
	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(fileName, &w, &h, &comp, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		AL_ENGINE_ERROR("GLCubeMap failed to load: {0}", fileName);
	}

	//setup framebuffers
	auto captureFBO = GLFrameBuffer(512, 512);
	captureFBO.Bind();
	captureFBO.AddDepthAttachment();
	if (glCheckNamedFramebufferStatus(captureFBO.ID(), GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		AL_ERROR("Framebuffer failed to compile");
	}

	//TEXTURE SETUP
	//capture/cubemap texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F,
			512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//irradiance map texture
	glGenTextures(1, &irradiance);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, 32, 32, 0,
			GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//prefilter map texture
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	/*//BRDF lut
	glGenTextures(1, &brdfLUTTexture);
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/


	//SHADER SETUP
	Shader EquirectToCubemapShader = Shader();
	std::string shaderPath("assets/shaders/PBR_EquirectToCubeMap.glsl");
	EquirectToCubemapShader.init(shaderPath);
	EquirectToCubemapShader.use();
	EquirectToCubemapShader.setInt("equirectangularMap", 0);
	EquirectToCubemapShader.setMat4("projection", captureProjection);

	Shader irradianceShader = Shader();
	std::string irradianceShaderPath("assets/shaders/PBR_Irradiance.glsl");
	irradianceShader.init(irradianceShaderPath);
	irradianceShader.use();
	irradianceShader.setInt("environmentMap", 0);
	irradianceShader.setMat4("projection", captureProjection);

	Shader preFilterShader = Shader();
	std::string preFilterShaderPath("assets/shaders/PBR_Prefilter.glsl");
	preFilterShader.init(preFilterShaderPath);
	preFilterShader.use();
	preFilterShader.setInt("environmentMap", 0);
	preFilterShader.setMat4("projection", captureProjection);

	/*
	Shader brdfShader = Shader();
	std::string brdfShaderPath("assets/shaders/PRB_BRDF.glsl");
	brdfShader.init(brdfShaderPath);
	brdfShader.use();*/

	//DRAW CALLS
	//EQUIRECT TO CUBEMAP
	captureFBO.Invalidate(512,512);
	captureFBO.Bind();
	EquirectToCubemapShader.use();
	cubeVAO.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);

	glDepthFunc(GL_LEQUAL);
	for (unsigned int i = 0; i < 6; ++i)
	{
		EquirectToCubemapShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	/*captureFBO.UnBind();*/
	glGenerateTextureMipmap(id);

	//IRRADIANCE MAP
	AL_ENGINE_TRACE("Generating Irradiance diffuse map for: {0} ", fileName);
	captureFBO.Invalidate(32, 32);
	captureFBO.Bind();
	irradianceShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//PREFILTER MAP
	captureFBO.Bind();
	preFilterShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		captureFBO.Invalidate(mipWidth, mipHeight);
		captureFBO.Bind();

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		preFilterShader.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			preFilterShader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	glDepthFunc(GL_LESS);
	captureFBO.UnBind();

	/*//BRDF LUT
	unsigned int FBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	auto quadVAO = GLVertexArray();
	quadVAO.Bind();
	auto quadVBO = GLVertexBuffer(nullptr, 0);
	BufferLayout layout;
	layout.AddAttribute({ "aPos", BufferAttribType::Float3, false });
	layout.AddAttribute({ "aTexCoord", BufferAttribType::Float2, false });
	quadVBO.SetLayout(layout);

	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	quadVBO.Unbind();
	quadVAO.UnBind();*/

	AL_ENGINE_INFO("Irradiance diffuse map for {0} created, cubemap loaded", fileName);
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
