#include "Renderer.h"

#include "Camera.h"
#include "Core/WindowManager.hpp"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECS.hpp"
#include "stb/stb_image.h"


Camera camera = Camera();

extern ECS ecs;
std::vector<Renderbatch> renderBatches;

/*Renderbatch renderBatch = Renderbatch();*/


void Renderer::entityAdded(Entity entity)
{
	
}

void Renderer::entityRemoved(Entity entity)
{
	
}

void Renderer::add(Entity entity)
{
	boolean added = false;
	for (auto &render_batch : renderBatches)
	{
		if (!(render_batch.indexCount >= Renderbatch::MAX_INDEX_COUNT))
		{
			render_batch.addSprite(ecs.GetComponent<Transform>(entity), ecs.GetComponent<SpriteRender>(entity));
			added = true;
		}
	}
	if (!added)
	{
		Renderbatch *newRenderBatch = new Renderbatch();
		newRenderBatch->init();
		newRenderBatch->beginBatch();
		newRenderBatch->addSprite(ecs.GetComponent<Transform>(entity), ecs.GetComponent<SpriteRender>(entity));
		renderBatches.push_back(*newRenderBatch);
	}
}

void Renderer::nonRenderBatchInit()
{
	//Create Shader
	std::string shaderPath("resources/shaders/Basic.glsl");
	shader = Shader();
	shader.init(shaderPath);
	shader.use();

	//Create Texture
	texture = loadTexture("resources/textures/container.jpg");
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// bind Texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// render container
	shader.use();
}

void Renderer::start()
{

	
	camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	for (auto &entity : mEntities)
	{
		add(entity);
	}
	
	/*nonRenderBatchInit();*/
}


void Renderer::update()
{

	for (auto &render_batch : renderBatches)
	{
		if (render_batch.batchEnded == false)
		{
			render_batch.endBatch();
			render_batch.batchEnded = true;
		}
		render_batch.flush();
	}
}



unsigned int Renderer::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

