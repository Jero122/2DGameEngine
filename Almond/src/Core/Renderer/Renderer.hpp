#pragma once
#include <iostream>

#include "Shader.h"
#include "Core/ECS/ECSTypes.hpp"
#include "GL/glew.h"
#include "SDL/SDL.h"
#include "stb/stb_image.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char * function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

class Renderer: public System
{

private:

	/*
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

    unsigned int texture; */
    Shader shader;
   

public:
	void start() override;
	void update() override;
	SDL_Window* mWindow;
	
};


