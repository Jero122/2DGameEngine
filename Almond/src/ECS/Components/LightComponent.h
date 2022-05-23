#pragma once
#include <glm/vec3.hpp>

struct LightComponent
{
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	enum Type {SpotLight, PointLight  };

	//SpotLight
	float constant;
	float linear;
	float quadratic;

	//PointLight
	glm::vec3 direction;
	float innerCutOff;
	float outerCutoff;
};
