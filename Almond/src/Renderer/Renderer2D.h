#pragma once
#include "ECS/Components/SpriteRender.h"
#include "ECS/Components/Transform.h"


class Renderer2D
{
public:
	static void Init();
	static void Shutdown();
	static void BeginScene();
	static void EndScene();
	static void Submit(Transform& transform, SpriteRender& spriteRender);

	struct RenderStats
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;

		uint32_t GetTotalVertexCount() { return  QuadCount * 4; }
		uint32_t GetTotalIndexCount() { return  QuadCount * 6; }
	};

	static void ResetStats();
	static RenderStats GetStats();

private:
	static void BeginBatch();
	static void EndBatch();
	static void NextBatch();
	static void Flush();
};

