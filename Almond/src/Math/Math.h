#pragma once
#include <glm/detail/type_mat2x2.hpp>

namespace Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}

