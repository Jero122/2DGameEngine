#include "RenderBatch.h"
#include "GLCall.h"
#include "Core/WindowManager.hpp"

void RenderBatch::init()
{
	std::string shaderPath("resources/shaders/Basic.glsl");
	shader = Shader();
	shader.init(shaderPath);

	vertexBuffer = new Vertex[MAX_VERTEX_COUNT];

	GLCALL(glGenVertexArrays(1, &VAO));
	GLCALL(glBindVertexArray(VAO));

	GLCALL(glGenBuffers(1, &VBO));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)0));
	GLCALL(glEnableVertexAttribArray(0));
	// color attribute
	GLCALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)(3 * sizeof(float))));
	GLCALL(glEnableVertexAttribArray(1));

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

	vertexBufferPtr = vertexBuffer;
	
	GLCALL(glGenBuffers(1, &EBO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
}

void RenderBatch::beginBatch()
{
	vertexBufferPtr = vertexBuffer;
	indexCount = 0;
}

void RenderBatch::endBatch()
{
	GLsizeiptr size = vertexBufferPtr - vertexBuffer;
	size = size * sizeof(Vertex);
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	/*GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * MAX_BATCH_COUNT, vertexBuffer, GL_DYNAMIC_DRAW));*/
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertexBuffer));
}

void RenderBatch::flush()
{
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);


	glm::mat4 view = WindowManager::instance().camera.GetViewMatrix();
	
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	//shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	GLCALL(glBindVertexArray(VAO));
	GLCALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
	//indexCount = 0;
}

void RenderBatch::drawQuad(const Transform& transform, const SpriteRender& sprite)
{
	glm::vec3 pos = transform.position;
	float width = sprite.width;
	float height = sprite.height;
	glm::vec4 col = sprite.colour;

	//top right
	Vertex v1 = Vertex{ glm::vec3(pos.x + width / 2,pos.y + height / 2,0), glm::vec4(col.r / 255,col.g / 255,col.b / 255, 1) };
	//bottom right
	Vertex v2 = Vertex{ glm::vec3(pos.x + width / 2,pos.y - height / 2,0), glm::vec4(col.r / 255,col.g / 255,col.b / 255, 1) };
	//bottom left
	Vertex v3 = Vertex{ glm::vec3(pos.x - width / 2,pos.y - height / 2,0), glm::vec4(col.r / 255,col.g / 255,col.b / 255, 1) };
	//top left
	Vertex v4 = Vertex{ glm::vec3(pos.x - width / 2,pos.y + height / 2,0), glm::vec4(col.r / 255,col.g / 255,col.b, 1) };


	vertexBufferPtr->Position = { pos.x , pos.y, 0 };
	vertexBufferPtr->Color = sprite.colour;
	vertexBufferPtr++;

	vertexBufferPtr->Position = { pos.x , pos.y - height, 0 };
	vertexBufferPtr->Color = sprite.colour;
	vertexBufferPtr++;

	vertexBufferPtr->Position = { pos.x - width , pos.y - height, 0 };
	vertexBufferPtr->Color = sprite.colour;
	vertexBufferPtr++;

	vertexBufferPtr->Position = { pos.x - width, pos.y, 0 };
	vertexBufferPtr->Color = sprite.colour;
	vertexBufferPtr++;

	indexCount += 6;
}


