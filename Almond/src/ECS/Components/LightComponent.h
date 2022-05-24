#pragma once
#include <glm/vec3.hpp>

struct LightComponent
{
	enum Type { SpotLight, PointLight };
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	Type type;

	//SpotLight
	float constant;
	float linear;
	float quadratic;

	//PointLight
	glm::vec3 direction;
	float innerCutOff;
	float outerCutoff;

	LightComponent() = default;

	LightComponent(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse,
		const glm::vec3& specular, float constant, float linear, float quadratic)
		: position(position),
		  ambient(ambient),
		  diffuse(diffuse),
		  specular(specular),
		  constant(constant),
		  linear(linear),
		  quadratic(quadratic)
	{
		type = PointLight;
		direction = { 0,0,0 };
		innerCutOff = 0;
		outerCutoff = 0;
	}

	LightComponent(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse,
		const glm::vec3& specular, const glm::vec3& direction, float inner_cut_off, float outer_cutoff)
		: position(position),
		  ambient(ambient),
		  diffuse(diffuse),
		  specular(specular),
		  direction(direction),
		  innerCutOff(inner_cut_off),
		  outerCutoff(outer_cutoff)
	{
		type = SpotLight;
		constant = 0;
		linear = 0;
		quadratic = 0;
	}
};
