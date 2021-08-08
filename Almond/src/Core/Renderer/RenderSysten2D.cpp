#include "RenderSysten2D.h"
#include "RenderBatch.h"

#include "GLCall.h"
#include <SDL/SDL.h>
#include "Core/ECS/ECS.hpp"
#include "stb/stb_image.h"



extern ECS ecs;




typedef struct Renderable
{
	Entity entity;
	Transform& transform;
	SpriteRender& sprite;

	Renderable(Entity entity, Transform& transform, SpriteRender& sprite)
		: entity(entity),
		  transform(transform),
		  sprite(sprite)
	{
	}

	Renderable(const Renderable& other)
		: entity(other.entity),
		  transform(other.transform),
		  sprite(other.sprite)
	{
	}


	Renderable& operator=(const Renderable& other)
	{
		if (this == &other)
			return *this;
		entity = other.entity;
		transform = other.transform;
		sprite = other.sprite;
		return *this;
	}
};

std::vector<Renderable> renderables;
RenderBatch dynamicRenderBatch;

void RenderSysten2D::EntityAdded(Entity entity)
{
	renderables.push_back({entity, ecs.GetComponent<Transform>(entity), ecs.GetComponent<SpriteRender>(entity) });
}

void RenderSysten2D::EntityRemoved(Entity entity)
{
	if (ecs.isAlive(entity))
	{
		for (auto iter = renderables.begin(); iter != renderables.end(); ++iter) {
			if (iter->entity == entity) {
				iter = renderables.erase(iter);
				break;
			}
		}
	}

}

void RenderSysten2D::Submit(Transform& transform, SpriteRender& spriteRender)
{
	if (dynamicRenderBatch.indexCount >= RenderBatch::MAX_INDEX_COUNT)
	{
		dynamicRenderBatch.endBatch();
		dynamicRenderBatch.flush();
		dynamicRenderBatch.BeginBatch();
		stats.DrawCalls++;
	}
	dynamicRenderBatch.Submit(transform, spriteRender);
	stats.QuadCount++;
}



void RenderSysten2D::Init()
{
	dynamicRenderBatch.Init();
}


void RenderSysten2D::Update()
{
	ResetRenderStats();


	for (auto renderable : renderables)
	{
			Submit(renderable.transform, renderable.sprite);
	}

	if (dynamicRenderBatch.indexCount >= 0)
	{
		dynamicRenderBatch.endBatch();
		dynamicRenderBatch.flush();
		dynamicRenderBatch.BeginBatch();
		stats.DrawCalls++;
	}
}

void RenderSysten2D::ShutDown()
{
	
}


void RenderSysten2D::ResetRenderStats()
{
	memset(&stats, 0, sizeof(RenderStats));
}

RenderStats RenderSysten2D::GetRenderStats() const
{
	return stats;
}

unsigned int RenderSysten2D::LoadTexture(char const* path)
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

