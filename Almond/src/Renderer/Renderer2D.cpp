#include <GL/glew.h>
#include <array>

#include "Renderer2D.h"
#include "Camera.h"
#include "GLCall.h"
#include "Shader.h"


extern Camera camera;

struct Quad
{
	struct Vertex
	{
		glm::vec3 Position;
		//glm::vec4 Color;
		unsigned int Color;
		glm::vec2 TexCoord;
		float TexID;
	};
	Vertex topRight;
	Vertex bottomRight;
	Vertex bottomLeft;
	Vertex topLeft;
};

struct RendererData
{
	static const int MAX_BATCH_COUNT = 10000;
	static const int MAX_VERTEX_COUNT = MAX_BATCH_COUNT * 4;
	static const int MAX_INDEX_COUNT = MAX_BATCH_COUNT * 6;

	static const int POS_COUNT = 3;			//XYZ
	static const int COLOR_COUNT = 4;		//RGBA
	static const int TEX_COORD_COUNT = 2;	//UV
	static const int TEX_ID_COUNT = 1;		//TexID


	Quad* m_QuadBuffer;
	Quad* m_QuadBufferPtr = nullptr;

	static const int s_MaxTextureSlots = 32;
	std::array<int, s_MaxTextureSlots> m_TextureSlots;
	unsigned int m_TextureSlotIndex = 1; //0 = white texture;

	unsigned int VAO, VBO, EBO;
	Shader shader;
	Renderer2D::RenderStats m_RenderStats;

	glm::vec4 m_Vertices[4];
	int indexCount = 0;
};

static RendererData s_Data;


void Renderer2D::Init()
{
	std::string shaderPath("resources/shaders/Basic.glsl");
	s_Data.shader = Shader();
	s_Data.shader.init(shaderPath);
	s_Data.shader.use();

	s_Data.m_QuadBuffer = new Quad[s_Data.MAX_BATCH_COUNT];

	GLCALL(glGenVertexArrays(1, &s_Data.VAO));
	GLCALL(glBindVertexArray(s_Data.VAO));

	GLCALL(glGenBuffers(1, &s_Data.VBO));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, s_Data.MAX_VERTEX_COUNT * sizeof(Quad::Vertex), nullptr, GL_DYNAMIC_DRAW));

	// POSITION
	GLCALL(glVertexAttribPointer(0, s_Data.POS_COUNT, GL_FLOAT, GL_FALSE, sizeof(Quad::Vertex), (void*)(offsetof(Quad::Vertex, Quad::Vertex::Position))));
	GLCALL(glEnableVertexAttribArray(0));
	//RGBA COLOR
	GLCALL(glVertexAttribPointer(1, s_Data.COLOR_COUNT, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Quad::Vertex), (void*)(offsetof(Quad::Vertex, Quad::Vertex::Color))));
	GLCALL(glEnableVertexAttribArray(1));
	//TEX UV
	GLCALL(glVertexAttribPointer(2, s_Data.TEX_COORD_COUNT, GL_FLOAT, GL_FALSE, sizeof(Quad::Vertex), (void*)(offsetof(Quad::Vertex, Quad::Vertex::TexCoord))));
	GLCALL(glEnableVertexAttribArray(2));
	//TEX ID
	GLCALL(glVertexAttribPointer(3, s_Data.TEX_ID_COUNT, GL_FLOAT, GL_FALSE, sizeof(Quad::Vertex), (void*)(offsetof(Quad::Vertex, Quad::Vertex::TexID))));
	GLCALL(glEnableVertexAttribArray(3));


	//EBO
	//Populates INDICES array as that will remain the same
	uint32_t indices[s_Data.MAX_INDEX_COUNT];
	uint32_t offset = 0;
	for (int i = 0; i < s_Data.MAX_INDEX_COUNT; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 3 + offset;

		indices[i + 3] = 1 + offset;
		indices[i + 4] = 2 + offset;
		indices[i + 5] = 3 + offset;

		offset += 4;
	}

	s_Data.m_QuadBufferPtr = s_Data.m_QuadBuffer;

	GLCALL(glGenBuffers(1, &s_Data.EBO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.EBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	int32_t samplers[s_Data.s_MaxTextureSlots];
	for (uint32_t i = 0; i < s_Data.s_MaxTextureSlots; ++i)
	{
		samplers[i] = i;
	}

	s_Data.m_Vertices[0] = { 0.5f, 0.5f, 0.0f, 1.0f }; // top right
	s_Data.m_Vertices[1] = { 0.5f, -0.5f, 0.0f, 1.0f };  // bottom right
	s_Data.m_Vertices[2] = { -0.5f, -0.5f, 0.0f, 1.0f };  // bottom left
	s_Data.m_Vertices[3] = { -0.5f,  0.5f, 0.0f, 1.0f };   // top left 


	s_Data.shader.setIntArray("uTextures", samplers, s_Data.s_MaxTextureSlots);
}

void Renderer2D::Shutdown()
{
	/*delete[] quadBuffer;*/
}

void Renderer2D::BeginScene()
{
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	BeginBatch();
}

void Renderer2D::EndScene()
{
	EndBatch();
	Flush();
}

void Renderer2D::Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID, glm::vec2* texCoords)
{
	glm::mat4 transformMatrix{ 1.0f };

	if (rotation == 0)
	{
		transformMatrix = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
	}
	else
	{
		transformMatrix = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
	}

	DrawQuad(transformMatrix, color, textureID, texCoords);

}


void Renderer2D::DrawQuad(const glm::mat4 transform, glm::vec4 color, int textureID, glm::vec2* texCoords)
{
	s_Data.m_RenderStats.QuadCount++;

	if (s_Data.indexCount >= s_Data.MAX_INDEX_COUNT)
	{
		NextBatch();
	}

	float textureIndex = 0;

	//If textureID is already in batch
	for (unsigned int i = 0; i < s_Data.m_TextureSlotIndex; ++i)
	{
		if (s_Data.m_TextureSlots[i] == textureID)
		{
			//Set the textureIndex for this quad to the one already in the batch;
			textureIndex = i;
			break;
		}
	}

	//If textureID was not in batch
	if (textureIndex == 0 && textureID > 0)
	{
		//If all the textureSlots have been occupied, end batch
		if (s_Data.m_TextureSlotIndex >= s_Data.s_MaxTextureSlots)
		{
			NextBatch();
		}

		textureIndex = s_Data.m_TextureSlotIndex;
		s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = textureID;
		s_Data.m_TextureSlotIndex++;
	}

	int r = color.r;
	int g = color.g;
	int b = color.b;
	int a = color.a;

	unsigned int c = a << 24 | b << 16 | g << 8 | r;

	//top right
	s_Data.m_QuadBufferPtr->topRight = Quad::Vertex{ transform * s_Data.m_Vertices[0], c, texCoords[0], textureIndex };
	//bottom right
	s_Data.m_QuadBufferPtr->bottomRight = Quad::Vertex{ transform * s_Data.m_Vertices[1],c, texCoords[1], textureIndex };
	//bottom left
	s_Data.m_QuadBufferPtr->bottomLeft = Quad::Vertex{ transform * s_Data.m_Vertices[2], c, texCoords[2], textureIndex };
	//top left
	s_Data.m_QuadBufferPtr->topLeft = Quad::Vertex{ transform * s_Data.m_Vertices[3],c, texCoords[3], textureIndex };

	s_Data.m_QuadBufferPtr++;

	s_Data.indexCount += 6;
}

void Renderer2D::BeginBatch()
{
	s_Data.m_QuadBufferPtr = s_Data.m_QuadBuffer;
	s_Data.indexCount = 0;
	s_Data.m_TextureSlotIndex = 1; //Set to 1 because 0 is reserved for colored quads
}

void Renderer2D::EndBatch()
{
	s_Data.m_RenderStats.DrawCalls++;

	GLsizeiptr size = s_Data.m_QuadBufferPtr - s_Data.m_QuadBuffer;
	size = size * sizeof(Quad);
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO));
	/*GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * MAX_BATCH_COUNT, vertexBuffer, GL_DYNAMIC_DRAW));*/
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.m_QuadBuffer));
}


void Renderer2D::Flush()
{
	s_Data.shader.use();

	//TODO Replace orthographic dimensions with a global aspect ratio setting
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::ortho(-8.0f / camera.fov, 8.0f / camera.fov, -4.5f / camera.fov, 4.5f / camera.fov, -1.0f, 1.0f);

	s_Data.shader.setMat4("uView", view);
	s_Data.shader.setMat4("uProjection", projection);

	for (unsigned int i = 1; i < s_Data.m_TextureSlotIndex; ++i)
	{
		GLCALL(glActiveTexture(GL_TEXTURE0 + i));
		glBindTexture(GL_TEXTURE_2D, s_Data.m_TextureSlots[i]);
	}

	GLCALL(glBindVertexArray(s_Data.VAO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.EBO));

	glDrawElements(GL_TRIANGLES, s_Data.indexCount, GL_UNSIGNED_INT, 0);
}


void Renderer2D::NextBatch()
{
	EndBatch();
	Flush();
	BeginBatch();
}

void Renderer2D::ResetStats()
{
	memset(&s_Data.m_RenderStats, 0, sizeof(RenderStats));
}

Renderer2D::RenderStats Renderer2D::GetStats()
{
	return s_Data.m_RenderStats;
}
