#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_MOVEMENT
{
	FORWARD, BACKWARDS, LEFT, RIGHT, UP, DOWN
};

class OldCamera
{
public: 
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 90.0f;
    float speed = 4.5f;
    float lookSensitivity = 0.1f;


    OldCamera() = default;
    OldCamera(const glm::mat4& m_projection);

   
    void Move(CAMERA_MOVEMENT movement, float deltaTime);
    void MouseMovement(float deltaX, float deltaY);
    void Zoom(float value);
	
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    void UpdateProjectionMatrix(float width, float height);


private:
    void updateCamera();
    glm::mat4 m_Projection;
	
};

