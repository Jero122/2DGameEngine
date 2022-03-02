#pragma once
#include "OldCamera.h"
#include "EditorCamera.h"
#include "Renderer.h"
#include "ECS/Components/SpriteRenderer.h"
#include "ECS/Components/Transform.h"
#include "glm/fwd.hpp"


class GPUBatched_Renderer: public Renderer
{
private:
	static void BeginBatch();
	static void EndBatch();
	static void NextBatch();
	static void Flush();
	static void DrawQuad(const glm::mat4 transform, glm::vec4 color, int textureID, glm::vec2* texCoords);
public:
	GPUBatched_Renderer();
	void Shutdown() override;
	void BeginScene(OldCamera& camera) override;
	void BeginScene(EditorCamera& camera) override;
	void EndScene() override;
	void Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID,
		glm::vec2* texCoords) override;
	void ResetStats() override;
	RenderStats GetStats() override;
};

