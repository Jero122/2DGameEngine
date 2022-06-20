#pragma once
#include <gl/glew.h>
class GLVertexArray
{
public:
	GLVertexArray()
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
