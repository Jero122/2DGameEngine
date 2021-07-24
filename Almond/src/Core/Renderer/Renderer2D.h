#pragma once
#include "Shader.h"
#include "Core/ECS/ECSTypes.hpp"

class Renderer2D :public System
{
private:

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	unsigned int texture;
	Shader shader;

	unsigned int indices[6] = {  // note that we start from 0!
		3, 2, 0,  // first Triangle
		0, 2, 1   // second Triangle
	};
	float vertices[32] = {
		// positions          //RGB colors        // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
	};

	unsigned loadTexture(const char* path);

public:

	Renderer2D() {};
	void start() override;
	void update() override;
	void entityAdded(Entity entity) override;
	void entityRemoved(Entity entity) override;
	void add(Entity entity);
	void nonRenderBatchInit();

	bool useBatching = true;
};

