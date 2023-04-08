#pragma once
#include <memory>
#include <vector>
#include <glm/vec2.hpp>
#include "glm/vec3.hpp"
#include "Renderer/Material.h"
#include "Renderer/Shader.h"
#include "Renderer/GLFramework/GLBuffer.h"
#include "Renderer/GLFramework/GLVertexArray.h"

class ProceduralMesh
{
public:
	std::vector<glm::vec3> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec2> m_UVs;
	std::shared_ptr<Material> material;

	ProceduralMesh(const std::vector<glm::vec3>& vertices, const std::vector<unsigned>& indices,
		const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uv,
		const std::shared_ptr<Material>& material);

	void GenerateNormals(const std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const std::vector<unsigned int>& indices);

	void GenerateUVs(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals,
		std::vector<glm::vec2>& uvs,const std::vector<unsigned int>& indices);

	void Draw(Shader& shader);
private:
	std::unique_ptr<GLVertexArray> m_VertexArray;
	std::unique_ptr<GLVertexBuffer> m_VertexBuffer;
	std::unique_ptr<GLIndexBuffer> m_IndexBuffer;

	void setupMesh();
};
