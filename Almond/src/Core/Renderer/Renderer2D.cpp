#include "Renderer2D.h"
#include "RenderBatch.h"

#include <vector>
#include "GLCall.h"
#include <SDL/SDL.h>

#include "Core/Window.h"
#include "Core/ECS/ECS.hpp"
#include "stb/stb_image.h"



extern ECS ecs;


std::unordered_map<uint32_t, RenderBatch> renderBatches;


void Renderer2D::entityAdded(Entity entity)
{
	/*add(entity);*/
}

void Renderer2D::entityRemoved(Entity entity)
{
	if (ecs.isAlive(entity))
	{
		//need to check if entity is valid
		auto spriteRender = ecs.GetComponent<SpriteRender>(entity);
		auto& batch = renderBatches.at(spriteRender.batchID);
		batch.removeQuad(entity);
	}

}

void Renderer2D::add(Entity entity)
{
	auto &transform = ecs.GetComponent<Transform>(entity);
	auto &spriteRender = ecs.GetComponent<SpriteRender>(entity);
	
	bool added = false;
	for (auto &pair : renderBatches)
	{
		auto& renderBatch = pair.second;
		if (!(renderBatch.indexCount >= RenderBatch::MAX_INDEX_COUNT))
		{
			renderBatch.drawQuad(entity, transform, spriteRender);
			spriteRender.batchID = renderBatch.id;
			added = true;
		}
	}
	if (!added)
	{
		RenderBatch*newRenderBatch = new RenderBatch();
		newRenderBatch->init();
		newRenderBatch->beginBatch();
		
		newRenderBatch->drawQuad(entity, transform, spriteRender);
		spriteRender.batchID = newRenderBatch->id;
		
		renderBatches.insert(std::make_pair(newRenderBatch->id, *newRenderBatch));

		stats.DrawCalls++;
	}
	stats.QuadCount++;
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

void Renderer2D::init()
{
	nonRenderBatchInit();
}


void Renderer2D::Update()
{
	resetRenderStats();

	for (auto entity : mEntities)
	{
		add(entity);
	}
	for (auto& pair : renderBatches)
	{
		auto& renderBatch = pair.second;
		renderBatch.endBatch();
		renderBatch.flush();
		renderBatch.beginBatch();
		stats.DrawCalls++;
	}
	
	stats.QuadCount = mEntities.size();
	/*if (useBatching)
	{
		for (auto& pair : renderBatches)
		{
			auto& renderBatch = pair.second;
			renderBatch.endBatch();
			renderBatch.flush();
			stats.DrawCalls++;
		}
	}*/
	//else
	{
		/*glm::mat4 view = Window::instance().camera.GetViewMatrix();
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
			
			stats.DrawCalls++;
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}*/
	}


	
}

void Renderer2D::shutdown()
{
	for (auto &pair : renderBatches)
	{
		auto render_batch = pair.second;
		render_batch.shutdown();
	}
}


void Renderer2D::resetRenderStats()
{
	memset(&stats, 0, sizeof(RenderStats));
}

RenderStats Renderer2D::getRenderStats()
{
	return stats;
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

