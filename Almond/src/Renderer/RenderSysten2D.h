#pragma once

#include "RenderBatch.h"
#include "Shader.h"
#include "ECS/ECSTypes.h"
#include "ECS/Components/SpriteRender.h"
#include "ECS/Components/Transform.h"


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
	
	RenderBatch GetRenderBatch() const
	{
		return m_RenderBatch;
	}
};

