#pragma once
#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>

class GLCubeMap
{
public:
	GLCubeMap(const char* fileName);
	GLCubeMap(std::vector<std::string> faces);

	~GLCubeMap();

	GLCubeMap(const GLCubeMap&) = delete;
	GLCubeMap(GLCubeMap&&);

	int GetWidth() const;
	int GetHeight() const;

	int ID() const;
	unsigned int id;

private:
	int m_Width;
	int m_Height;
};
