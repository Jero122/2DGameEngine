#pragma once
#include "Shader.h"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECSTypes.hpp"

typedef struct RenderStats
{
	uint32_t DrawCalls = 0;
	uint32_t QuadCount = 0;

	uint32_t GetTotalVertexCount() { return  QuadCount * 4; }
	uint32_t GetTotalIndexCount() { return  QuadCount * 6; }
};

class RenderSysten2D :public System
{
private:

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	Shader shader;
	RenderStats stats;

	void Submit(Transform& transform, SpriteRender& spriteRender);

public:
	RenderSysten2D() {};
	void Init() override;
	void ShutDown() override;
	
	void Update() override;
	void EntityAdded(Entity entity) override;
	void EntityRemoved(Entity entity) override;
	
	
	RenderStats GetRenderStats() const;
	void ResetRenderStats();
};

