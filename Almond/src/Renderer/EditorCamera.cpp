#include "EditorCamera.h"

EditorCamera::EditorCamera(float fov, float aspectRatio, float near, float far)
	:m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(near), m_FarClip(far)
{
	UpdateProjection();
	UpdateView();
}

void EditorCamera::UpdateProjection()
{
	m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
	m_Projection = glm::perspective(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);

	//PROJECTION IS FLIPPED UPSIDE DOWN
	m_Projection = glm::scale(m_Projection, { 1,-1,1 });
}

void EditorCamera::UpdateView()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(direction);
	
	m_Right = glm::normalize(glm::cross(m_Front, m_Up));
	auto centre = m_Position - m_Front;

	m_ViewMatrix = glm::inverse(glm::lookAt(m_Position, centre, m_Up));
}


void EditorCamera::SetViewPortSize(float width, float height)
{
	m_ViewportWidth = width;
	m_ViewportHeight = height;
	UpdateProjection();
}


void EditorCamera::OnUpdate(TimeStep timeStep)
{
	
}
