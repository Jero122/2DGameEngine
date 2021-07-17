#include <string>
#include <sstream>
#include <glm/glm.hpp>

#pragma once
class Shader
{
public:
	unsigned int ID;
	Shader() {};
	void init(std::string& sourcePath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec3(const std::string& name, float  x, float y, float z) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec4(const std::string& name, float  x, float y, float z, float w) const;

private:
	unsigned int compileShader(unsigned int type, const std::string& source);
	void createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
};

