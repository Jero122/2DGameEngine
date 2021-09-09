#include "RenderBatch.h"
#include "GLCall.h"
#include "Core/Window.h"

extern Camera camera;

RenderBatch::RendererData RenderBatch::s_Data;

void RenderBatch::Init()
{
	std::string shaderPath("resources/shaders/Basic.glsl");
	s_Data.shader = Shader();
	s_Data.shader.init(shaderPath);
	s_Data.shader.use();

	s_Data.m_QuadBuffer = new Quad[MAX_BATCH_COUNT];

	GLCALL(glGenVertexArrays(1, &s_Data.VAO));
	GLCALL(glBindVertexArray(s_Data.VAO));

	GLCALL(glGenBuffers(1, &s_Data.VBO));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Quad::Vertex), nullptr, GL_DYNAMIC_DRAW));

	// POSITION
	GLCALL(glVertexAttribPointer(0, POS_COUNT, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)POS_OFFSET));
	GLCALL(glEnableVertexAttribArray(0));
	//RGBA COLOR
	GLCALL(glVertexAttribPointer(1, COLOR_COUNT, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)(COLOR_OFFSET * sizeof(float))));
	GLCALL(glEnableVertexAttribArray(1));
	//TEX UV
	GLCALL(glVertexAttribPointer(2, TEX_COORD_COUNT, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)(TEX_COORD_OFFSET * sizeof(float))));
	GLCALL(glEnableVertexAttribArray(2));
	//TEX ID
	GLCALL(glVertexAttribPointer(3, TEX_ID_COUNT, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)(TEX_ID_OFFSET * sizeof(float))));
	GLCALL(glEnableVertexAttribArray(3));
	

	//EBO
	//Populates INDICES array as that will remain the same
	uint32_t indices[MAX_INDEX_COUNT];
	uint32_t offset = 0;
	for (int i = 0; i < MAX_INDEX_COUNT; i += 6)
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

void RenderBatch::BeginBatch()
{
	s_Data.m_QuadBufferPtr = s_Data.m_QuadBuffer;
	s_Data.indexCount = 0;
	s_Data.m_TextureSlotIndex = 1; //Set to 1 because 0 is reserved for colored quads
}

void RenderBatch::EndBatch()
{
	s_Data.m_RenderStats.DrawCalls++;
	
	GLsizeiptr size = s_Data.m_QuadBufferPtr - s_Data.m_QuadBuffer;
	size = size * sizeof(Quad);
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, s_Data.VBO));
	/*GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * MAX_BATCH_COUNT, vertexBuffer, GL_DYNAMIC_DRAW));*/
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.m_QuadBuffer));
}

void RenderBatch::Flush()
{
	s_Data.shader.use();


	//TODO Replace orthographic dimensions with a global aspect ratio setting
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::ortho(-8.0f/camera.fov, 8.0f / camera.fov, -4.5f / camera.fov, 4.5f / camera.fov, -1.0f, 1.0f);
	
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


void RenderBatch::NextBatch()
{
	EndBatch();
	Flush();
	BeginBatch();
}

void RenderBatch::Submit(const Transform& transform, const SpriteRender& sprite)
{
	s_Data.m_RenderStats.QuadCount++;
	
	if (s_Data.indexCount >= MAX_INDEX_COUNT)
	{
		NextBatch();
	}

	float textureIndex = 0;

	//If textureID is already in batch
	for (unsigned int i = 0; i < s_Data.m_TextureSlotIndex; ++i)
	{
		if (s_Data.m_TextureSlots[i] == sprite.textureID)
		{
			//Set the textureIndex for this quad to the one already in the batch;
			textureIndex = i;
			break;
		}
	}

	//If textureID was not in batch
	if (textureIndex == 0 && sprite.textureID > 0)
	{
		//If all the textureSlots have been occupied, end batch
		if (s_Data.m_TextureSlotIndex >= s_Data.s_MaxTextureSlots)
		{
			NextBatch();
		}

		textureIndex = s_Data.m_TextureSlotIndex;
		s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = sprite.textureID;
		s_Data.m_TextureSlotIndex++;
	}

	glm::mat4 transFormMatrix = glm::translate(glm::mat4(1.0f), transform.position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), { 0.0f,0.0f,1.0f })
		* glm::scale(glm::mat4(1.0f), { sprite.width * transform.scale.x, sprite.height * transform.scale.y, 1.0f });
	
	
	//top right
	s_Data.m_QuadBufferPtr->topRight = Quad::Vertex{ transFormMatrix * s_Data.m_Vertices[0], glm::vec4(sprite.color.r / 255,sprite.color.g / 255,sprite.color.b / 255, 1), sprite.texCoords[0], textureIndex};
	//bottom right
	s_Data.m_QuadBufferPtr->bottomRight = Quad::Vertex{ transFormMatrix * s_Data.m_Vertices[1], glm::vec4(sprite.color.r / 255,sprite.color.g / 255,sprite.color.b / 255, 1), sprite.texCoords[1], textureIndex };
	//bottom left
	s_Data.m_QuadBufferPtr->bottomLeft = Quad::Vertex{ transFormMatrix * s_Data.m_Vertices[2], glm::vec4(sprite.color.r / 255,sprite.color.g / 255,sprite.color.b / 255, 1), sprite.texCoords[2], textureIndex };
	//top left
	s_Data.m_QuadBufferPtr->topLeft= Quad::Vertex{transFormMatrix * s_Data.m_Vertices[3], glm::vec4(sprite.color.r / 255,sprite.color.g / 255,sprite.color.b, 1), sprite.texCoords[3], textureIndex };

	s_Data.m_QuadBufferPtr++;
	
	s_Data.indexCount += 6;

}




void RenderBatch::Shutdown()
{
	/*delete[] quadBuffer;*/
}

