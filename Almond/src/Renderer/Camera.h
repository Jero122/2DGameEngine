#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(const glm::mat4& projection)
		: m_Projection(projection) {}

	virtual ~Camera() = default;

	const glm::mat4 GetProjection() { return m_Projection; }
protected:
	glm::mat4 m_Projection = glm::mat4(1.0f);
};