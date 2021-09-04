#include "RenderSysten2D.h"
#include "RenderBatch.h"

#include "GLCall.h"
#include <SDL/SDL.h>
#include "ECS/ECS.hpp"
#include "ECS/SceneView.h"
#include "stb/stb_image.h"



extern ECS ecs;



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
	for (EntityID ent : SceneView<Transform, SpriteRender>(*ecs.getEntityManager()))
	{
		Transform* pTransform = ecs.GetComponent<Transform>(ent);
		SpriteRender* pSprite = ecs.GetComponent<SpriteRender>(ent);
		Submit(*pTransform, *pSprite);
	}

	if (m_RenderBatch.indexCount >= 0)
	{
		m_RenderBatch.NextBatch();
	}
}

void RenderSysten2D::ShutDown()
{
	
}