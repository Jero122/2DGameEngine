#pragma once
#include <string>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "GLFramework/GLBuffer.h"
#include "GLFramework/GLRenderCommand.h"
#include "GLFramework/GLVertexArray.h"
#include "glm/vec3.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices,
		const std::vector<Texture>& textures)
		: vertices(vertices),
		indices(indices),
		textures(textures)
	{
		setupMesh();
	}


	void Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		std::vector<unsigned int> texIDs;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//glBindTextureUnit(i, textures[i].id);
			texIDs.push_back(textures[i].ID());
		}
		GLRenderCommand::BindTextures(0, textures.size(), texIDs.data());

		// draw mesh
		m_VertexArray->Bind();
		GLRenderCommand::DrawElementsTriangle(indices.size(), 0);
		m_VertexArray->UnBind();
	}
private:
	std::unique_ptr<GLVertexArray> m_VertexArray;
	std::unique_ptr<GLVertexBuffer> m_VertexBuffer;
	std::unique_ptr<GLIndexBuffer> m_IndexBuffer;

	void setupMesh()
	{
		m_VertexArray = std::make_unique<GLVertexArray>();
		m_VertexArray->Bind();
		m_VertexBuffer = std::make_unique<GLVertexBuffer>((float*)(&vertices[0]), vertices.size() * sizeof(Vertex));
		BufferLayout layout;
		layout.AddAttribute({ "aPos", BufferAttribType::Float3, false });
		layout.AddAttribute({ "aNormal", BufferAttribType::Float3, true });
		layout.AddAttribute({ "aTexCoord", BufferAttribType::Float2, false });
		m_VertexBuffer->SetLayout(layout);
		m_IndexBuffer = std::make_unique<GLIndexBuffer>(&indices[0], indices.size() * sizeof(unsigned int));
		m_IndexBuffer->Bind();
		m_VertexArray->UnBind();
	}
};
