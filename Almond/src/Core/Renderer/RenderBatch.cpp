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
	indexCount = 0;
}

void RenderBatch::endBatch()
{
	//if (isDirty)
	{
		auto& array = quadArray.getComponentArray();
		GLsizeiptr quadArrSize = (quadArray.mCurrentEntityIndex + 1) * sizeof(Quad);
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, quadArrSize, &array[0]));
		isDirty = false;
	}
}

void RenderBatch::flush()
{
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);


	Camera camera = Camera();
	camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = camera.GetViewMatrix();
	
	
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	//shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	GLCALL(glBindVertexArray(VAO));
	GLCALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
	//indexCount = 0;
}

void RenderBatch::drawQuad(const Entity& entity,const Transform& transform, const SpriteRender& sprite)
{
	glm::vec3 pos = transform.position;
	float width = sprite.width;
	float height = sprite.height;
	glm::vec4 col = sprite.colour;

	//top right
	Quad::Vertex v1 = Quad::Vertex{ glm::vec3(pos.x + width / 2,pos.y + height / 2,0), glm::vec4(col.r / 255,col.g / 255,col.b / 255, 1) };
	//bottom right
	Quad::Vertex v2 = Quad::Vertex{ glm::vec3(pos.x + width / 2,pos.y - height / 2,0), glm::vec4(col.r / 255,col.g / 255,col.b / 255, 1) };
	//bottom left
	Quad::Vertex v3 = Quad::Vertex{ glm::vec3(pos.x - width / 2,pos.y - height / 2,0), glm::vec4(col.r / 255,col.g / 255,col.b / 255, 1) };
	//top left
	Quad::Vertex v4 = Quad::Vertex{ glm::vec3(pos.x - width / 2,pos.y + height / 2,0), glm::vec4(col.r / 255,col.g / 255,col.b, 1) };

	Quad quad = Quad{ v1,v2,v3,v4 };
	
	quadArray.insertData(entity,quad);

	isDirty = true;
	indexCount += 6;
}

void RenderBatch::removeQuad(Entity entity)
{

	quadArray.removeData(entity);
	indexCount -= 6;
	isDirty = true;
}


void RenderBatch::shutdown()
{
	/*delete[] quadBuffer;*/
}

