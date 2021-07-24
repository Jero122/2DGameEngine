#pragma once
#include "Camera.h"
#include "Shader.h"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"

class RenderBatch
{
public:
	int indexCount = 0;
	bool batchEnded = false;
	static const int MAX_BATCH_COUNT = 20000;
	static const int MAX_VERTEX_COUNT = MAX_BATCH_COUNT * 4;
	static const int MAX_INDEX_COUNT = MAX_BATCH_COUNT * 6;
	RenderBatch(){}

	void init();
	void beginBatch();
	void endBatch();
	void flush();
	void drawQuad(const Transform& transform, const SpriteRender& sprite);
private:
	typedef struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};
	
	const int POS_COUNT = 3;	//XYZ
	const int COLOR_COUNT = 4;	//RGBA
	const int POS_OFFSET = 0;
	const int COLOR_OFFSET = POS_OFFSET + POS_COUNT;
	const int VERTEX_ELEMENT_COUNT = 7;
	const int VERTEX_SIZE = VERTEX_ELEMENT_COUNT * sizeof(float);

	unsigned int VAO, VBO, EBO;
	Shader shader;

	Vertex* vertexBuffer = nullptr;
	Vertex* vertexBufferPtr = nullptr;

	
	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
};
