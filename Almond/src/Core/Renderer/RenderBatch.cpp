#include "RenderBatch.h"
#include "GLCall.h"
#include "Core/Window.h"

void RenderBatch::Init()
{
	std::string shaderPath("resources/shaders/Basic.glsl");
	shader = Shader();
	shader.init(shaderPath);
	shader.use();

	m_QuadBuffer = new Quad[MAX_BATCH_COUNT];

	GLCALL(glGenVertexArrays(1, &VAO));
	GLCALL(glBindVertexArray(VAO));

	GLCALL(glGenBuffers(1, &VBO));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Quad::Vertex), nullptr, GL_DYNAMIC_DRAW));

	// POSITIOn
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
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	m_QuadBufferPtr = m_QuadBuffer;
	
	GLCALL(glGenBuffers(1, &EBO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	int32_t samplers[s_MaxTextureSlots];
	for (uint32_t i = 0; i < s_MaxTextureSlots; ++i)
	{
		samplers[i] = i;
	}

	shader.setIntArray("uTextures", samplers, s_MaxTextureSlots);

}

void RenderBatch::BeginBatch()
{
	m_QuadBufferPtr = m_QuadBuffer;
	indexCount = 0;
	m_TextureSlotIndex = 1; //Set to 1 because 0 is reserved for colored quads
}

void RenderBatch::endBatch()
{
	GLsizeiptr size = m_QuadBufferPtr - m_QuadBuffer;
	size = size * sizeof(Quad);
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	/*GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * MAX_BATCH_COUNT, vertexBuffer, GL_DYNAMIC_DRAW));*/
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_QuadBuffer));
}

void RenderBatch::flush()
{
	shader.use();


	Camera camera = Camera();
	camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = camera.GetViewMatrix();
	
	shader.setMat4("uView", view);
	shader.setMat4("uProjection", projection);

	for (int i = 1; i < m_TextureSlotIndex; ++i)
	{
		GLCALL(glActiveTexture(GL_TEXTURE0 + i));
		glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
	}
	
	GLCALL(glBindVertexArray(VAO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}


void RenderBatch::NextBatch()
{
	endBatch();
	flush();
	BeginBatch();
}

void RenderBatch::Submit(const Transform& transform, const SpriteRender& sprite)
{

	if (indexCount >= MAX_INDEX_COUNT)
	{
		NextBatch();
	}

	float textureIndex = 0;

	//If textureID is already in batch
	for (int i = 0; i < m_TextureSlotIndex; ++i)
	{
		if (m_TextureSlots[i] == sprite.textureID)
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
		if (m_TextureSlotIndex >= s_MaxTextureSlots)
		{
			NextBatch();
		}

		textureIndex = m_TextureSlotIndex;
		m_TextureSlots[m_TextureSlotIndex] = sprite.textureID;
		m_TextureSlotIndex++;
	}
	
	//top right
	m_QuadBufferPtr->topRight = Quad::Vertex{ glm::vec3(transform.position.x + sprite.width / 2,transform.position.y + sprite.height / 2,0), glm::vec4(sprite.color.r / 255,sprite.color.g / 255,sprite.color.b / 255, 1), glm::vec2(1,1), textureIndex};
	//bottom right
	m_QuadBufferPtr->bottomRight = Quad::Vertex{ glm::vec3(transform.position.x + sprite.width / 2,transform.position.y - sprite.height / 2,0), glm::vec4(sprite.color.r / 255,sprite.color.g / 255,sprite.color.b / 255, 1), glm::vec2(1,0), textureIndex };
	//bottom left
	m_QuadBufferPtr->bottomLeft = Quad::Vertex{ glm::vec3(transform.position.x - sprite.width / 2,transform.position.y - sprite.height / 2,0), glm::vec4(sprite.color.r / 255,sprite.color.g / 255,sprite.color.b / 255, 1), glm::vec2(0,0), textureIndex };
	//top left
	m_QuadBufferPtr->topLeft= Quad::Vertex{ glm::vec3(transform.position.x - sprite.width / 2,transform.position.y + sprite.height / 2,0), glm::vec4(sprite.color.r / 255,sprite.color.g / 255,sprite.color.b, 1), glm::vec2(0,1), textureIndex };

	m_QuadBufferPtr++;
	
	indexCount += 6;

}

void RenderBatch::removeQuad(Entity entity)
{

	/*quadArray.removeData(entity);
	indexCount -= 6;
	isDirty = true;*/
}


void RenderBatch::shutdown()
{
	/*delete[] quadBuffer;*/
}

