#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SDL/SDL.h>

#include "Camera.h"
#include "Shader.h"
#include "Core/Components/SpriteRender.h"
#include "Core/Components/Transform.h"
#include "Core/ECS/ECSTypes.hpp"


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}



static const int MAX_BATCH_COUNT = 20000;
static const int MAX_VERTEX_COUNT = MAX_BATCH_COUNT * 4;
static const int MAX_INDEX_COUNT = MAX_BATCH_COUNT * 6;

typedef struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
};


class Renderbatch
{
private:
	const int POS_COUNT = 3;	//XYZ
	const int COLOR_COUNT = 4;	//RGBA
	const int POS_OFFSET = 0;
	const int COLOR_OFFSET = POS_OFFSET + POS_COUNT;
	const int VERTEX_ELEMENT_COUNT = 7;
	const int VERTEX_SIZE = VERTEX_ELEMENT_COUNT * sizeof(float);

	unsigned int VAO, VBO, EBO;
	Shader shader;
	Camera camera;

	int numSprite = 0;

	int indexCount = 0;
	Vertex* vertexBuffer = nullptr;
	Vertex* vertexBufferPtr = nullptr;
	
	/*
	std::vector<Vertex> verticesVector;
	std::vector<SpriteRender> sprites;*/
	
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);


public:
	Renderbatch()
	{
		
	}

	void init()
	{
		std::cout << sizeof(Vertex) << std::endl;
		std::string shaderPath("resources/shaders/Basic.glsl");
		shader = Shader();
		shader.init(shaderPath);

		vertexBuffer = new Vertex[MAX_VERTEX_COUNT];

		GLCALL(glGenVertexArrays(1, &VAO));
		GLCALL(glGenBuffers(1, &VBO));

		GLCALL(glBindVertexArray(VAO));
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));

		GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, vertexBuffer, GL_DYNAMIC_DRAW));

		/*GLCALL(glVertexAttribPointer(0, POS_COUNT, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)(POS_OFFSET * sizeof(float))));
		GLCALL(glEnableVertexAttribArray(0));
		GLCALL(glVertexAttribPointer(1, COLOR_COUNT, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)(COLOR_OFFSET * sizeof(float))));
		GLCALL(glEnableVertexAttribArray(1));*/

		GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)0));
		GLCALL(glEnableVertexAttribArray(0));
		// color attribute
		GLCALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_ELEMENT_COUNT * sizeof(float), (void*)(3 * sizeof(float))));
		GLCALL(glEnableVertexAttribArray(1));

		//EBO
		//Populates INDICES array as that will remain the same
		uint32_t indices[MAX_INDEX_COUNT];
		uint32_t offset = 0;
		for (int i = 0; i < MAX_INDEX_COUNT; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}


		vertexBufferPtr = vertexBuffer;

		GLCALL(glGenBuffers(1, &EBO));
		GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
		GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

		
	}

	void beginBatch()
	{
		vertexBufferPtr = vertexBuffer;
	}

	void endBatch()
	{
		GLsizeiptr size = vertexBufferPtr - vertexBuffer;
		size = size * sizeof(Vertex);
		GLCALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4 * MAX_BATCH_COUNT, vertexBuffer, GL_DYNAMIC_DRAW));
		GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertexBuffer));
	}

	void flush()
	{
		
		shader.use();
		shader.setMat4("view", view);
		//shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		
		GLCALL(glBindVertexArray(VAO));
		GLCALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
		//indexCount = 0;
	}
	
	void generateVertices(Transform transform, SpriteRender sprite)
	{
		glm::vec3 pos = transform.position;
		float width = sprite.width;
		float height = sprite.height;
		glm::vec4 col = sprite.colour;

		//top right
		Vertex v1 = Vertex{ glm::vec3(pos.x + width/2,pos.y + height/2,0), glm::vec4(col.r/255,col.g/255,col.b/255, 1) };
		//bottom right
		Vertex v2 = Vertex{ glm::vec3(pos.x + width / 2,pos.y - height / 2,0), glm::vec4(col.r/255,col.g/255,col.b/255, 1) };
		//bottom left
		Vertex v3 = Vertex{ glm::vec3(pos.x - width / 2,pos.y - height / 2,0), glm::vec4(col.r/255,col.g/255,col.b/255, 1) };
		//top left
		Vertex v4 = Vertex{ glm::vec3(pos.x - width / 2,pos.y + height / 2,0), glm::vec4(col.r/255,col.g/255,col.b, 1) };

		
		vertexBufferPtr->Position = { pos.x , pos.y, 0 };
		vertexBufferPtr->Color = sprite.colour;
		vertexBufferPtr++;

		vertexBufferPtr->Position = { pos.x , pos.y - height, 0 };
		vertexBufferPtr->Color = sprite.colour;
		vertexBufferPtr++;

		vertexBufferPtr->Position = { pos.x - width , pos.y - height, 0 };
		vertexBufferPtr->Color = sprite.colour;
		vertexBufferPtr++;

		vertexBufferPtr->Position = { pos.x - width, pos.y, 0 };
		vertexBufferPtr->Color = sprite.colour;
		vertexBufferPtr++;

		indexCount += 6;

	}


	void addSprite(const Transform &transform, const SpriteRender &sprite)
	{
		/*sprites.push_back(sprite);*/
		generateVertices(transform, sprite);
	}


private:
};


class Renderer :public System
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
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned loadTexture(const char* path);

public:

	Renderer() {};
	void start() override;
	void update() override;
	void entityAdded(Entity entity) override;
	void entityRemoved(Entity entity) override;
	void renderBatchInit();
	void nonRenderBatchInit();
};

