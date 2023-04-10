#include "ProceduralMesh.h"

#include "Renderer/GLFramework/GLRenderCommand.h"

ProceduralMesh::ProceduralMesh(const std::vector<glm::vec3>& vertices, const std::vector<unsigned>& indices,
                               const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uv, const std::shared_ptr<Material>& material)

	: m_Vertices(vertices),
	m_Indices(indices),
	m_Normals(normals),
	m_UVs(uv),
	material(material)
{
	if (m_Normals.empty())
	{
		//GenerateNormals(m_Vertices,m_Normals, m_Indices);
	}

	if (m_UVs.empty())
	{
		//GenerateUVs(m_Vertices, m_Normals, m_UVs, m_Indices);
	}

	setupMesh();
}

void ProceduralMesh::GenerateNormals()

{
	// Initialize normal vectors
	m_Normals.resize(m_Vertices.size(), glm::vec3(0.0f));

	// Calculate normal for each face
	for (unsigned int i = 0; i < m_Indices.size(); i += 3)
	{
		glm::vec3 v1 = m_Vertices[m_Indices[i]];
		glm::vec3 v2 = m_Vertices[m_Indices[i + 1]];
		glm::vec3 v3 = m_Vertices[m_Indices[i + 2]];

		glm::vec3 normal = glm::cross(v2 - v1, v3 - v1);
		normal *= -1;

		m_Normals[m_Indices[i]] += normal;
		m_Normals[m_Indices[i + 1]] += normal;
		m_Normals[m_Indices[i + 2]] += normal;
	}

	// Normalize normal vectors
	for (unsigned int i = 0; i < m_Normals.size(); i++)
		m_Normals[i] = glm::normalize(m_Normals[i]);
}

void ProceduralMesh::GenerateUVs()
{
	m_UVs.resize(m_Vertices.size());
	for (unsigned int i = 0; i < m_Vertices.size(); i++)
	{
		glm::vec3 position = m_Vertices[i];

		float u, v;
		if (fabs(position.x) > fabs(position.y) && fabs(position.x) > fabs(position.z))
		{
			// X is largest
			if (position.x > 0)
			{
				u = position.z / position.x + 1.0f;
				v = position.y / position.x + 1.0f;
			}
			else
			{
				u = -position.z / position.x + 1.0f;
				v = position.y / position.x + 1.0f;
			}
		}
		else if (fabs(position.y) > fabs(position.x) && fabs(position.y) > fabs(position.z))
		{
			// Y is largest
			if (position.y > 0)
			{
				u = position.x / position.y + 1.0f;
				v = position.z / position.y + 1.0f;
			}
			else
			{
				u = position.x / position.y + 1.0f;
				v = -position.z / position.y + 1.0f;
			}
		}
		else
		{
			// Z is largest
			if (position.z > 0)
			{
				u = position.x / position.z + 1.0f;
				v = position.y / position.z + 1.0f;
			}
			else
			{
				u = -position.x / position.z + 1.0f;
				v = position.y / position.z + 1.0f;
			}
		}

		u /= 2.0f;
		v /= 2.0f;

		m_UVs[i] = glm::vec2(u, v);
	}
}

void ProceduralMesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	int texCount = 4;

	std::vector<unsigned int> texIDs;

	texIDs.push_back(material->albedoMap->ID());
	texIDs.push_back(material->aoRoughnessMetallicMap->ID());
	texIDs.push_back(material->normalMap->ID());
	texIDs.push_back(material->emissiveMap->ID());

	GLRenderCommand::BindTextures(3, texCount, texIDs.data());
	shader.setVec4("material.albedoColour", material->albedoColour);
	shader.setVec4("material.emissiveColour", material->emissiveColour);
	shader.setFloat("material.metallic", material->metallic);
	shader.setFloat("material.roughness", material->roughness);

	// draw mesh
	m_VertexArray->Bind();
	//Draw wireframe
	shader.setFloat("isWireframe", 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	GLRenderCommand::DrawElementsTriangle(m_Indices.size(), 0);

	//Draw mesh
	shader.setFloat("isWireframe", 0.0f);
	// Draw mesh with polygon offset
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLRenderCommand::DrawElementsTriangle(m_Indices.size(), 0);
	glDisable(GL_POLYGON_OFFSET_FILL);
	m_VertexArray->UnBind();

	GLRenderCommand::BindTextures(1, texCount, 0);
}

void ProceduralMesh::setupMesh()
{
	m_VertexArray = std::make_unique<GLVertexArray>();
	m_VertexArray->Bind();

	//Create Buffer Data, arranged in GPU mmory according to vector order
	std::vector<GLVertexBuffer::BufferData> bufferData = {
		{(float*)(m_Vertices.data()), (uint32_t)(m_Vertices.size() * sizeof(glm::vec3))},
		{(float*)(m_Normals.data()), (uint32_t)(m_Normals.size() * sizeof(glm::vec3))},
		{(float*)(m_UVs.data()), (uint32_t)(m_UVs.size() * sizeof(glm::vec2))}
	};


	//Create VBO with data
	m_VertexBuffer = std::make_unique<GLVertexBuffer>(bufferData);
	//Specify an attribute layout for VBO
	BufferLayout layout;
	layout.AddAttribute({ "aPos", BufferAttribType::Float3, false });
	layout.AddAttribute({ "aNormal", BufferAttribType::Float3, true });
	layout.AddAttribute({ "aTexCoord", BufferAttribType::Float2, false });
	m_VertexBuffer->SetLayout(layout);
	//Create index/element buffer
	m_IndexBuffer = std::make_unique<GLIndexBuffer>(&m_Indices[0], m_Indices.size() * sizeof(unsigned int));

	m_IndexBuffer->Bind();
	m_VertexArray->UnBind();
}