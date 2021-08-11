#pragma once
#include "Camera.h"
#include "Shader.h"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/PackedArray.hpp"
#include <boost/unordered/unordered_map.hpp>

static int currentID = 0;

class RenderBatch
{
public:

	int indexCount = 0;
	int id;
	
	
	
	static const int MAX_BATCH_COUNT = 10000;
	static const int MAX_VERTEX_COUNT = MAX_BATCH_COUNT * 4;
	static const int MAX_INDEX_COUNT = MAX_BATCH_COUNT * 6;
	RenderBatch()
	{
		id = currentID;
		currentID++;
	}

	void Init();
	void shutdown();
	
	void BeginBatch();
	void endBatch();
	void flush();
	void NextBatch();
	
	void Submit(const Transform& transform, const SpriteRender& sprite);
	void removeQuad(Entity entity);

	struct Quad
	{
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexID;
		};
		Vertex topRight;
		Vertex bottomRight;
		Vertex bottomLeft;
		Vertex topLeft;
	};

	
	
	
private:
	static const int POS_COUNT = 3;			//XYZ
	static const int COLOR_COUNT = 4;		//RGBA
	static const int TEX_COORD_COUNT = 2;	//UV
	static const int TEX_ID_COUNT = 1;		//TexID

	static const int POS_OFFSET = 0;
	static const int COLOR_OFFSET = POS_OFFSET + POS_COUNT;
	static const int TEX_COORD_OFFSET = COLOR_OFFSET + COLOR_COUNT;
	static const int TEX_ID_OFFSET = TEX_COORD_OFFSET + TEX_COORD_COUNT;


	static const int VERTEX_ELEMENT_COUNT = POS_COUNT + COLOR_COUNT + TEX_COORD_COUNT + TEX_ID_COUNT;
	static const int VERTEX_SIZE = VERTEX_ELEMENT_COUNT * sizeof(float);

	static const int s_MaxTextureSlots = 2;


	Quad* m_QuadBuffer;
	Quad* m_QuadBufferPtr = nullptr;

	std::array<int, s_MaxTextureSlots> m_TextureSlots;
	unsigned int m_TextureSlotIndex = 32; //0 = white texture;
	

	


	unsigned int VAO, VBO, EBO;
	Shader shader;
	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
};
