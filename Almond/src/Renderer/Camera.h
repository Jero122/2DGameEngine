#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
	Camera() = default;
	Camera(const glm::mat4& m_projection)
		: m_Projection(m_projection)
	{
	}

	virtual ~Camera() = default;

	const glm::mat4 GetProjectionMatrix() { return  m_Projection; }
	const glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
	
protected:
	glm::mat4 m_Projection = glm::mat4(1.0f);
	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
};
