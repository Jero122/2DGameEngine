#include "RenderBatch.h"
#include "GLCall.h"
#include "Core/Window.h"

void RenderBatch::init()
{
	std::string shaderPath("resources/shaders/Basic.glsl");
	shader = Shader();
	shader.init(shaderPath);

	//quadBuffer = new Quad[MAX_BATCH_COUNT];

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

	GLCALL(glGenBuffers(1, &EBO));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
}

void RenderBatch::beginBatch()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	quadBuffer = (Quad*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void RenderBatch::endBatch()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
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

	GLCALL(glBindVertexArray(0));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	indexCount = 0;
}

void RenderBatch::drawQuad(const Transform& transform, const SpriteRender& sprite)
{

	//top right
	quadBuffer->topRight = Quad::Vertex{ glm::vec3(transform.position.x + sprite.width / 2,transform.position.y + sprite.height / 2,0), glm::vec4(sprite.colour.r / 255,sprite.colour.g / 255,sprite.colour.b / 255, 1) };
	//bottom right
	quadBuffer->bottomRight = Quad::Vertex{ glm::vec3(transform.position.x + sprite.width / 2,transform.position.y - sprite.height / 2,0), glm::vec4(sprite.colour.r / 255,sprite.colour.g / 255,sprite.colour.b / 255, 1) };
	//bottom left
	quadBuffer->bottomLeft = Quad::Vertex{ glm::vec3(transform.position.x - sprite.width / 2,transform.position.y - sprite.height / 2,0), glm::vec4(sprite.colour.r / 255,sprite.colour.g / 255,sprite.colour.b / 255, 1) };
	//top left
	quadBuffer->topLeft= Quad::Vertex{ glm::vec3(transform.position.x - sprite.width / 2,transform.position.y + sprite.height / 2,0), glm::vec4(sprite.colour.r / 255,sprite.colour.g / 255,sprite.colour.b, 1) };

	quadBuffer++;
	
	isDirty = true;
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

