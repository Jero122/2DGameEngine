#pragma once
#pragma once
#include "OldCamera.h"
#include "EditorCamera.h"
#include "glm/fwd.hpp"


class Renderer
{
public:
	virtual ~Renderer() = default;
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	//TODO remove OldCamera
	virtual void BeginScene(OldCamera& camera) = 0;
	virtual void BeginScene(EditorCamera& camera) = 0;
	virtual void EndScene() = 0;
	virtual void Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID, glm::vec2* texCoords) = 0;

	struct RenderStats
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t GetTotalVertexCount() { return  QuadCount * 4; }
		uint32_t GetTotalIndexCount() { return  QuadCount * 6; }
	};



	virtual void ResetStats() = 0;
	virtual RenderStats GetStats() = 0;

};

