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

	unsigned int texture;
	Shader shader;

	RenderStats stats;

	unsigned int indices[6] = {  // note that we start from 0!
		3, 2, 0,  // first Triangle
		0, 2, 1   // second Triangle
	};
	float vertices[32] = {
		// positions          //RGB colors        // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
	};

	unsigned LoadTexture(const char* path);
	void Submit(Transform& transform, SpriteRender& spriteRender);
	void nonRenderBatchInit();
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

