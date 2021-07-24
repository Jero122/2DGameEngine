#include "Renderer2D.h"
#include "RenderBatch.h"

#include <vector>
#include "GLCall.h"
#include <SDL/SDL.h>

#include "Core/WindowManager.hpp"
#include "Core/ECS/ECS.hpp"
#include "stb/stb_image.h"


extern ECS ecs;
std::vector<RenderBatch> renderBatches;



void Renderer2D::entityAdded(Entity entity)
{
	
}

void Renderer2D::entityRemoved(Entity entity)
{
	
}

void Renderer2D::add(Entity entity)
{
	bool added = false;
	for (auto &render_batch : renderBatches)
	{
		if (!(render_batch.indexCount >= RenderBatch::MAX_INDEX_COUNT))
		{
			render_batch.drawQuad(ecs.GetComponent<Transform>(entity), ecs.GetComponent<SpriteRender>(entity));
			added = true;
		}
	}
	if (!added)
	{
		RenderBatch*newRenderBatch = new RenderBatch();
		newRenderBatch->init();
		newRenderBatch->beginBatch();
		newRenderBatch->drawQuad(ecs.GetComponent<Transform>(entity), ecs.GetComponent<SpriteRender>(entity));
		renderBatches.push_back(*newRenderBatch);
	}
}

void Renderer2D::nonRenderBatchInit()
{
	//Create Shader
	std::string shaderPath("resources/shaders/Basic.glsl");
	shader = Shader();
	shader.init(shaderPath);
	shader.use();

	
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

	// render container
	shader.use();
}

void Renderer2D::start()
{
	if (useBatching)
	{
		for (auto& entity : mEntities)
		{
			add(entity);
		}
	}
	else
	{
		nonRenderBatchInit();
	}
}


void Renderer2D::update()
{
	if (useBatching)
	{
		for (auto& render_batch : renderBatches)
		{
			if (render_batch.batchEnded == false)
			{
				render_batch.endBatch();
				render_batch.batchEnded = true;
			}
			render_batch.flush();
		}
	}
	else
	{
		glm::mat4 view = WindowManager::instance().camera.GetViewMatrix();
		glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);


		for (auto& const entity : mEntities)
		{
			auto& transform = ecs.GetComponent<Transform>(entity);
			auto& spriteRender = ecs.GetComponent<SpriteRender>(entity);

			shader.use();
			//SPRITE SIZE
			glm::vec2 size = glm::vec2(spriteRender.width, spriteRender.height);

			//VIEWSPACE ORIGIN
			glm::vec3 viewSpaceTransform = glm::vec3(transform.position.x , transform.position.y , transform.position.z);
			glm::mat4 model = glm::mat4(1.0f);
			//SPRITE ORIGIN
			model = glm::translate(model, viewSpaceTransform);
			model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
			model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
			model = glm::scale(model, glm::vec3(size, 1.0f));

			shader.setMat4("model", model);
			shader.setMat4("view", view);
			//shader.setMat4("view", view);
			shader.setMat4("projection", projection);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}


	
}



unsigned int Renderer2D::loadTexture(char const* path)
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

