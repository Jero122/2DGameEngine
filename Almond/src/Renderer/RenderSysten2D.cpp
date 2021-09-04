#include "RenderSysten2D.h"
#include "RenderBatch.h"
#include "ECS/ECS.hpp"
#include "ECS/SceneView.h"

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
	
	for (EntityID ent : SceneView<Transform, SpriteRender>(ecs))
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