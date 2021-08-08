#include "RenderBatch.h"
#include "GLCall.h"
#include "Core/Window.h"

void RenderBatch::Init()
{
	std::string shaderPath("resources/shaders/Basic.glsl");
	shader = Shader();
	shader.init(shaderPath);

	m_QuadBuffer = new Quad[MAX_BATCH_COUNT];

	GLCALL(glGenVertexArrays(1, &VAO));
	GLCALL(glBindVertexArray(VAO));

	GLCALL(glGenBuffers(1, &VBO));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Quad::Vertex), nullptr, GL_DYNAMIC_DRAW));

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

	m_QuadBufferPtr = m_QuadBuffer;
	
	GLCALL(glGenBuffers(1, &EBO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
}

void RenderBatch::BeginBatch()
{
	m_QuadBufferPtr = m_QuadBuffer;
	indexCount = 0;
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
	
	
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	GLCALL(glBindVertexArray(VAO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	indexCount = 0;
}

void RenderBatch::Submit(const Transform& transform, const SpriteRender& sprite)
{
	//top right
	m_QuadBufferPtr->topRight = Quad::Vertex{ glm::vec3(transform.position.x + sprite.width / 2,transform.position.y + sprite.height / 2,0), glm::vec4(sprite.colour.r / 255,sprite.colour.g / 255,sprite.colour.b / 255, 1) };
	//bottom right
	m_QuadBufferPtr->bottomRight = Quad::Vertex{ glm::vec3(transform.position.x + sprite.width / 2,transform.position.y - sprite.height / 2,0), glm::vec4(sprite.colour.r / 255,sprite.colour.g / 255,sprite.colour.b / 255, 1) };
	//bottom left
	m_QuadBufferPtr->bottomLeft = Quad::Vertex{ glm::vec3(transform.position.x - sprite.width / 2,transform.position.y - sprite.height / 2,0), glm::vec4(sprite.colour.r / 255,sprite.colour.g / 255,sprite.colour.b / 255, 1) };
	//top left
	m_QuadBufferPtr->topLeft= Quad::Vertex{ glm::vec3(transform.position.x - sprite.width / 2,transform.position.y + sprite.height / 2,0), glm::vec4(sprite.colour.r / 255,sprite.colour.g / 255,sprite.colour.b, 1) };

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

