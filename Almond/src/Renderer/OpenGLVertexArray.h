#pragma once
#include <gl/glew.h>
class OpenGLVertexArray
{
public:
	OpenGLVertexArray()
	{
		glGenVertexArrays(1, &ID);
	}
	void Bind()
	{
		glBindVertexArray(ID);
	}

	void UnBind()
	{
		glBindVertexArray(0);
	}
private:
	unsigned int ID;
};
