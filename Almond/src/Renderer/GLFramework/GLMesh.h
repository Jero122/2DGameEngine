#pragma once
#include <cstdint>
#include <memory>

#include "GLBuffer.h"
#include "GLRenderCommand.h"
#include "GLVertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

class GLMesh
{
public:
	GLMesh(uint32_t* indices, uint32_t indicesSizeBytes, float* vertexData, uint32_t verticesSizeBytes)
		:m_NumIndices(indicesSizeBytes / sizeof(uint32_t)),
		m_VertexBuffer(vertexData, verticesSizeBytes),
		m_IndexBuffer(indices, indicesSizeBytes),
		texture("assets/Models/Diffuse.jpg")

	{
		//GL Objects Init
		m_VertexArray.Bind();
		BufferLayout layout;
		layout.AddAttribute({"aPos", BufferAttribType::Float3, false});
		layout.AddAttribute({ "aNormal", BufferAttribType::Float3, true });
		layout.AddAttribute({ "aTexCoord", BufferAttribType::Float2, false });
		m_VertexBuffer.SetLayout(layout);
		m_IndexBuffer.Bind();
		m_VertexBuffer.Bind();
		m_IndexBuffer.UnBind();
		m_VertexBuffer.Unbind();
	}

	void Draw()
	{
		/*unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		std::vector<unsigned int> texIDs;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//glBindTextureUnit(i, textures[i].id);
			texIDs.push_back(textures[i].ID());
		}*/

		std::vector<unsigned int> texIDs;
		texIDs.push_back(texture.ID());
		

		GLRenderCommand::BindTextures(1, 1, texIDs.data());
		// draw mesh
		m_VertexArray.Bind();
		GLRenderCommand::DrawElementsTriangle(static_cast<GLsizei>(m_NumIndices), nullptr);
		m_VertexArray.UnBind();

		GLRenderCommand::BindTextures(1, 1, 0);
	}

private:
	GLVertexArray m_VertexArray;
	GLVertexBuffer m_VertexBuffer;
	GLIndexBuffer m_IndexBuffer;

	Texture texture;

	uint32_t m_NumIndices;
};
