#pragma once
#include "Camera.h"
#include "ECS/Components/SpriteRender.h"
#include "ECS/Components/Transform.h"
#include "glm/fwd.hpp"


class Renderer2D
{
public:
	static void Init();
	static void Shutdown();
	static void BeginScene(Camera& camera);
	static void EndScene();
	static void Submit(const glm::vec3 position,float rotation, glm::vec2 scale, glm::vec4 color, int textureID, glm::vec2* texCoords);

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
	static void DrawQuad(const glm::mat4 transform, glm::vec4 color, int textureID, glm::vec2* texCoords);
};

