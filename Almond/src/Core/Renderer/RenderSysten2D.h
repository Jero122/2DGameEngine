#pragma once
#include "RenderBatch.h"
#include "Shader.h"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECSTypes.hpp"



class RenderSysten2D :public System
{
private:

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	RenderBatch m_RenderBatch;
	
	Shader shader;

	void Submit(Transform& transform, SpriteRender& spriteRender);

public:
	RenderSysten2D() {};
	void Init() override;
	void ShutDown() override;
	
	void Update() override;
	void EntityAdded(Entity entity) override;
	void EntityRemoved(Entity entity) override;
	
	RenderBatch GetRenderBatch() const
	{
		return m_RenderBatch;
	}
};

