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
	dynamicRenderBatch.Submit(transform, spriteRender);
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
		dynamicRenderBatch.NextBatch();
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

