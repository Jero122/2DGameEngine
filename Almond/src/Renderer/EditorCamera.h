#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "Core/TimeStep.h"

class EditorCamera: public Camera
{
public:
	EditorCamera() = default;
	EditorCamera(float fov, float aspectRatio, float near, float far);
	glm::vec3 Position()
	{
		return m_Position;
	}
	void SetViewPortSize(float width, float height);
	virtual void OnUpdate(TimeStep timeStep);

private:
	void UpdateProjection();
	void UpdateView();
	
	glm::vec3 m_Position = {0.0f, 0.0f, -10.0f};
	glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_AspectRatio = 1.778f;

	float m_Speed = 4.5f;
	float m_LookSensitivity = 10.5f;
	float m_PanSensitivity = 1.0f;
	float m_ZoomSensitivity = 1.0f;
	
	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;
	float m_Fov = 45.0f;

	float m_NearClip = 0.1f; float m_FarClip = 100.0f;

	float m_ViewportHeight = 1280;
	float m_ViewportWidth = 720;

	
};

