#pragma once
#include <array>
#include <future>
#include "EditorCamera.h"
#include "Renderer.h"
#include "glm/fwd.hpp"


class Renderer2D: public Renderer
{
private:
	/*
	 * Measure these batch counts:
	 * 1000,2000,3000,4000,5000,10000,20000,40000
	 */
	static const uint32_t MAX_BATCH_COUNT = 4000;
	static const uint32_t MAX_VERTEX_COUNT = MAX_BATCH_COUNT * 4;
	static const uint32_t MAX_INDEX_COUNT = MAX_BATCH_COUNT * 6;
	static const uint32_t MAX_TEXTURE_SLOTS = 32;
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
			Quads.reserve(MAX_BATCH_COUNT);
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


	std::vector<std::future<void>> m_Futures;
	inline static std::mutex mutex;

	Batch* m_CurrentBatch;

	inline static auto m_BatchBufferData = std::make_shared<std::vector<BatchBuffer*>>();
	inline static auto m_Batches = std::make_shared<std::vector<Batch*>>();


	inline static glm::vec4 m_Vertices[4];

	void DrawQuad(const glm::mat4 transform, glm::vec4 color, int textureID, glm::vec2* texCoords);
	static void ProcessBatch(std::shared_ptr<std::vector<BatchBuffer*>> buffers, Batch* batch);
	static void ProcessBatches(std::shared_ptr<std::vector<BatchBuffer*>> buffers, std::shared_ptr<std::vector<Batch*>> m_Batches)
	{
		for (auto batch : *m_Batches)
		{
			ProcessBatch(buffers, batch);
		}
	}



public:
	Renderer2D();
	void Shutdown() override;
	void BeginScene(EditorCamera& camera) override;

	void EndScene() override;
	void Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID,
		glm::vec2* texCoords);
	void ResetStats() override;
	RenderStats GetStats() override;
};

