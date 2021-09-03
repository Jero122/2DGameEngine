#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_MOVEMENT
{
	FORWARD, BACKWARDS, LEFT, RIGHT, UP, DOWN
};

class Camera
{
public: 
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
    float speed = 2.5f;
    float lookSensitivity = 0.1f;

    void init(glm::vec3 position, glm::vec3 front, glm::vec3 up);
    void Move(CAMERA_MOVEMENT movement, float deltaTime);
    void MouseMovement(float deltaX, float deltaY);
    void Zoom(float value);
    glm::mat4 GetViewMatrix();

	
private:
    void updateCamera();
};

