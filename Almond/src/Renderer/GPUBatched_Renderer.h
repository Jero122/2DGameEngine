#pragma once
#include <array>

#include "OldCamera.h"
#include "EditorCamera.h"
#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"
#include "Renderer.h"
#include "Shader.h"
#include "ECS/Components/SpriteRenderer.h"
#include "ECS/Components/Transform.h"
#include "glm/fwd.hpp"



class GPUBatched_Renderer: public Renderer
{
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

private:
	void BeginBatch();
	void EndBatch();
	void NextBatch();
	void Flush();
	void DrawQuad(const glm::mat4 transform, glm::vec4 color, int textureID, glm::vec2* texCoords);

	static const int MAX_BATCH_COUNT = 10000;
	static const int MAX_VERTEX_COUNT = MAX_BATCH_COUNT * 4;
	static const int MAX_INDEX_COUNT = MAX_BATCH_COUNT * 6;

	Quad* m_QuadBuffer;
	Quad* m_QuadBufferPtr = nullptr;

	std::unique_ptr<OpenGLVertexArray> m_VertexArray;
	std::unique_ptr<OpenGLVertexBuffer> m_VertexBuffer;
	std::unique_ptr<OpenGLIndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;

	RenderStats m_RenderStats;

	static const int s_MaxTextureSlots = 32;
	std::array<int, 32> m_TextureSlots;
	unsigned int m_TextureSlotIndex = 1; //0 = white texture;
	glm::vec4 m_Vertices[4];
	int indexCount = 0;

	glm::mat4 projection;
	glm::mat4 view;
	float fov;
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

