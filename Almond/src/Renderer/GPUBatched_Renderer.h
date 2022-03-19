#pragma once
#include <array>

#include "EditorCamera.h"
#include "Renderer.h"
#include "glm/fwd.hpp"

class GPUBatched_Renderer: public Renderer
{
private:
	/*
	 * Measure these batch counts:
	 * 1000,2000,3000,4000,5000,10000,20000,40000
	 */
	static const int MAX_BATCH_COUNT = 40000;
	static const int MAX_VERTEX_COUNT = MAX_BATCH_COUNT * 4;
	static const int MAX_INDEX_COUNT = MAX_BATCH_COUNT * 6;

	struct Quad
	{
		struct Vertex
		{
			glm::vec3 VertexPosition;
			glm::vec3 Position;
			glm::vec2 Scale;
			float rotation;
			unsigned int Color;
			glm::vec2 TexCoord;
			float TexID;
		};
		Vertex topRight;
		Vertex bottomRight;
		Vertex bottomLeft;
		Vertex topLeft;
	};

	void BeginBatch();
	void EndBatch();
	void NextBatch();
	void Flush();
	void DrawQuad(const glm::mat4 transform, glm::vec4 color, int textureID, glm::vec2* texCoords);

	Quad* m_QuadBuffer;
	Quad* m_QuadBufferPtr = nullptr;
	RenderStats m_RenderStats;

	static const int s_MaxTextureSlots = 32;
	std::array<int, 32> m_TextureSlots;
	unsigned int m_TextureSlotIndex = 1; //0 = white texture;
	glm::vec4 m_Vertices[4];
	int indexCount = 0;
public:
	GPUBatched_Renderer();
	void Shutdown() override;
	void BeginScene(EditorCamera& camera) override;
	void EndScene() override;
	void Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID,
		glm::vec2* texCoords) override;
	void ResetStats() override;
	RenderStats GetStats() override;
};

