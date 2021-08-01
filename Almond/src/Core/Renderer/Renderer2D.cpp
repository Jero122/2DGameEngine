#include "Renderer2D.h"
#include "RenderBatch.h"

#include "GLCall.h"
#include <SDL/SDL.h>
#include "Core/ECS/ECS.hpp"
#include "stb/stb_image.h"



extern ECS ecs;


std::unordered_map<uint32_t, RenderBatch> renderBatches;
typedef struct Renderable
{
	Transform& transform;
	SpriteRender& sprite;
};

std::vector<Renderable> renderables;

void Renderer2D::entityAdded(Entity entity)
{
	renderables.push_back({ ecs.GetComponent<Transform>(entity), ecs.GetComponent<SpriteRender>(entity) });
}

void Renderer2D::entityRemoved(Entity entity)
{
	/*if (ecs.isAlive(entity))
	{
		//need to check if entity is valid
		auto spriteRender = ecs.GetComponent<SpriteRender>(entity);
		auto& batch = renderBatches.at(spriteRender.batchID);
		batch.removeQuad(entity);
	}*/

}

void Renderer2D::add(Transform& transform, SpriteRender& spriteRender)
{
	bool added = false;
	for (auto &pair : renderBatches)
	{
		auto& renderBatch = pair.second;
	
		
		if (!(renderBatch.indexCount >= RenderBatch::MAX_INDEX_COUNT))
		{
			renderBatch.drawQuad(transform, spriteRender);
			added = true;
		}
	}
	if (!added)
	{
		RenderBatch*newRenderBatch = new RenderBatch();
		newRenderBatch->init();
		newRenderBatch->beginBatch();
		
		newRenderBatch->drawQuad(transform, spriteRender);
		
		renderBatches.insert(std::make_pair(newRenderBatch->id, *newRenderBatch));

		stats.DrawCalls++;
	}
	stats.QuadCount++;
}



void Renderer2D::init()
{
}


void Renderer2D::Update()
{
	resetRenderStats();


	for (auto renderable : renderables)
	{
		add( renderable.transform, renderable.sprite);
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

