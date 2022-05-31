#pragma once
#include <string>
#include <vector>

#include "OpenGLRenderCommand.h"
#include "Shader.h"
#include "Texture.h"
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
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			OpenGLRenderCommand::BindTexture(textures[i].GetTexID(), i);
			/*glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)
			glBindTexture(GL_TEXTURE_2D, textures[i].GetTexID());*/

			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			//shader.setFloat(("material." + name + number).c_str(), i);
		}

		// draw mesh
		m_VertexArray->Bind();
		OpenGLRenderCommand::DrawElementsTriangle(indices.size(), 0);
		m_VertexArray->UnBind();
	}
private:
	std::unique_ptr<OpenGLVertexArray> m_VertexArray;
	std::unique_ptr<OpenGLVertexBuffer> m_VertexBuffer;
	std::unique_ptr<OpenGLIndexBuffer> m_IndexBuffer;

	void setupMesh()
	{
		m_VertexArray = std::make_unique<OpenGLVertexArray>();
		m_VertexArray->Bind();
		m_VertexBuffer = std::make_unique<OpenGLVertexBuffer>((float*)(&vertices[0]), vertices.size() * sizeof(Vertex));
		BufferLayout layout;
		layout.AddAttribute({ "aPos", BufferAttribType::Float3, false });
		layout.AddAttribute({ "aNormal", BufferAttribType::Float3, true });
		layout.AddAttribute({ "aTexCoord", BufferAttribType::Float2, false });
		m_VertexBuffer->SetLayout(layout);
		m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(&indices[0], indices.size() * sizeof(unsigned int));
		m_IndexBuffer->Bind();
		m_VertexArray->UnBind();
	}
};
