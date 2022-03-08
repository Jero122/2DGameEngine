#pragma once
#include <array>
#include "EditorCamera.h"
#include "Renderer.h"
#include "glm/fwd.hpp"


class CPUBatched_Renderer: public Renderer
{
private:
	struct Quad
	{
		struct Vertex
		{
			glm::vec3 Position;
			unsigned int Color;
			glm::vec2 TexCoord;
			float TexID;
		};
		Vertex topRight;
		Vertex bottomRight;
		Vertex bottomLeft;
		Vertex topLeft;
	};

	struct Batch
	{
		struct BatchQuad
		{
			glm::vec3 Position;
			float Rotation;
			glm::vec2 Scale;
			glm::vec4 Color;
			int TextureID;
			glm::vec2* TexCoords;
		};

		int QuadCount = 0;
		std::vector<BatchQuad> Quads;
		std::array<int, 32> TextureSlots;
		unsigned int TextureSlotIndex = 1; //0 = white texture;

		Batch()
		{
			Quads.reserve(10000);
		}

		void AddQuad(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID,
			glm::vec2* texCoords)
		{
			Quads.push_back({ position,rotation,scale,color,textureID,texCoords });
			QuadCount++;
		}

		void Clear()
		{
			Quads.clear();
			QuadCount = 0;
		}
	};

	struct BatchBuffer
	{
		Quad* QuadBuffer;
		Quad* QuadBufferPtr = nullptr;
		std::array<int, 32> TextureSlots;
		unsigned int CurrentTextureSlotIndex = 1; //0 = white texture;
		int IndexCount = 0;

		BatchBuffer()
		{
			QuadBuffer = new Quad[MAX_BATCH_COUNT];
			QuadBufferPtr = QuadBuffer;
		}
		~BatchBuffer()
		{
			QuadBufferPtr = nullptr;
			delete[] QuadBuffer;
		}

	};

	std::vector<Batch> m_Batches;
	Batch m_CurrentBatch;
	inline static std::vector<BatchBuffer*> m_BatchBufferData;
	inline static glm::vec4 m_Vertices[4];

	void DrawQuad(const glm::mat4 transform, glm::vec4 color, int textureID, glm::vec2* texCoords);
	static void ProcessBatch(std::vector<BatchBuffer*>* buffers,Batch batch);

	static const int MAX_BATCH_COUNT = 10000;
	static const int MAX_VERTEX_COUNT = MAX_BATCH_COUNT * 4;
	static const int MAX_INDEX_COUNT = MAX_BATCH_COUNT * 6;
	static const int MAX_TEXTURE_SLOTS = 32;

	RenderStats m_RenderStats;

public:
	CPUBatched_Renderer();
	void Shutdown() override;
	void BeginScene(EditorCamera& camera) override;

	void EndScene() override;
	void Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID,
		glm::vec2* texCoords) override;
	void ResetStats() override;
	RenderStats GetStats() override;
};

