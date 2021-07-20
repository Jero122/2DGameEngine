#include "Camera.h"



void Camera::init(glm::vec3 position, glm::vec3 front, glm::vec3 up)
{
	this->position = position;
	this->front = front;
	this->up = up;
	this->right = glm::normalize(glm::cross(front, up));
}

void Camera::Move(CAMERA_MOVEMENT movement, float deltaTime)
{
	const float velocity = speed * deltaTime;
	switch (movement)
	{
	case FORWARD:
		position += velocity * front;
		break;
	case BACKWARDS:
		position -= velocity * front;
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


void Camera::MouseMovement(float deltaX, float deltaY)
{
	yaw += deltaX * lookSensitivity;
	pitch -= deltaY * lookSensitivity;
	updateCamera();
}

void Camera::Zoom(float value)
{
	fov -= value;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
	updateCamera();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::updateCamera()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(front, up));
}
