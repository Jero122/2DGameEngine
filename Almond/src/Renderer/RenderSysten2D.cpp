#include "RenderSysten2D.h"
#include "RenderBatch.h"

#include "GLCall.h"
#include <SDL/SDL.h>
#include "ECS/ECS.hpp"
#include "stb/stb_image.h"



extern ECS ecs;

struct PhysicsObject
{
	Entity entity;
	Transform& transform;
	SpriteRender& sprite;

	PhysicsObject(Entity entity, Transform& transform, SpriteRender& sprite)
		: entity(entity),
		  transform(transform),
		  sprite(sprite)
	{
	}

	PhysicsObject(const PhysicsObject& other)
		: entity(other.entity),
		  transform(other.transform),
		  sprite(other.sprite)
	{
	}


	PhysicsObject& operator=(const PhysicsObject& other)
	{
		if (this == &other)
			return *this;
		entity = other.entity;
		transform = other.transform;
		sprite = other.sprite;
		return *this;
	}
};

std::vector<PhysicsObject> renderables;


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
	m_RenderBatch.Submit(transform, spriteRender);
}

void RenderSysten2D::Init()
{
	m_RenderBatch.Init();
}


void RenderSysten2D::Update()
{
	m_RenderBatch.ResetRenderStats();
	
	for (auto renderable : renderables)
	{
		Submit(renderable.transform, renderable.sprite);
	}

	if (m_RenderBatch.indexCount >= 0)
	{
		m_RenderBatch.NextBatch();
	}
}

void RenderSysten2D::ShutDown()
{
	
}