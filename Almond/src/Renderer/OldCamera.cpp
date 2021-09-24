#include "OldCamera.h"

#include <iostream>


OldCamera::OldCamera(const glm::mat4& m_projection)
	:m_Projection(m_projection)
{
	position = glm::vec3{ 0,0,10.0f };
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	
	right = glm::normalize(glm::cross(front, up));
	fov = 1.0f;
}

void OldCamera::Move(CAMERA_MOVEMENT movement, float deltaTime)
{
	const float velocity = speed * deltaTime;
	switch (movement)
	{
	case FORWARD:
		position += velocity * front;
		break;
	case BACKWARDS:
		position -= velocity * front * glm::vec3{0,0, 1};
		break;
	case LEFT:
		position -= right * velocity;
		break;
	case RIGHT:
		position += right * velocity;
		break;
	case UP:
		position += up * velocity;
		break;
	case DOWN:
		position -= up * velocity;
		break;
	}
	updateCamera();
}


void OldCamera::MouseMovement(float deltaX, float deltaY)
{
	yaw += deltaX * lookSensitivity;
	pitch -= deltaY * lookSensitivity;
	updateCamera();
}

void OldCamera::Zoom(float value)
{
	fov -= value;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
	updateCamera(); 
}

glm::mat4 OldCamera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 OldCamera::GetProjectionMatrix()
{
	return m_Projection;
}


void OldCamera::UpdateProjectionMatrix(float width, float height)
{
	float aspectRatio = width / height;
	m_Projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
	//m_Projection = glm::ortho(-aspectRatio, aspectRatio, 1.0f, -1.0f, -1.0f, 1.0f);
}

void OldCamera::updateCamera()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(front, up));

	std::cout << "Camera: " << position.z;
}
