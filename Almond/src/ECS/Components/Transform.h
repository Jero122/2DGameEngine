#pragma once
#include "glm/vec3.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct Transform
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 GetTransform() const
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(rotation));

		return glm::translate(glm::mat4(1.0f), position)
			* rotation
			* glm::scale(glm::mat4(1.0f), scale);
	}
};
