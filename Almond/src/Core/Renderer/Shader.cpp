#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <GL/glew.h> // include glad to get all the required OpenGL headers

Shader::Shader(std::string& sourcePath)
{

	std::ifstream stream(sourcePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType shaderType = ShaderType::NONE;
	std::string line;
	std::stringstream stringStream[2];
	
	while (getline(stream,line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				shaderType = ShaderType::VERTEX;
			}
			if (line.find("fragment") != std::string::npos)
			{
				shaderType = ShaderType::FRAGMENT;
			}
		}
		else
		{
			stringStream[int(shaderType)] << line << '\n';
		}
	}
	stream.close();

	createShaderProgram(stringStream[0].str(), stringStream[1].str());
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()),1,glm::value_ptr(value));
}


void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	float values[] = { x,y, z };
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, values);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	float values[] = { x,y, z, w};
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, values);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned Shader::compileShader(unsigned type, const std::string& source)
{
	//create shader
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	//Attaches source code to shader
	glShaderSource(id, 1, &src, nullptr);
	//Compiles the shader
	glCompileShader(id);

	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "SHADER DID NOT COMPILE: " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

void Shader::createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	//Creates a shader program
	ID = glCreateProgram();
	//Compiles the shaders
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//Attaches compiled shaders to shader program
	glAttachShader(ID, vs);
	glAttachShader(ID, fs);
	//Links attached shaders into one final shader program
	glLinkProgram(ID);
	glValidateProgram(ID);
	//Delete the shaders as we now have a program (shaders are intermediate files basically)
	glDeleteShader(vs);
	glDeleteShader(fs);
}
