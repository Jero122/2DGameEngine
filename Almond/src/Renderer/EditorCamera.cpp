#include "EditorCamera.h"

#include <GL/glew.h>

#include "Core/Input.h"

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
	//gluPerspective(m_Fov, m_AspectRatio, m_NearClip, m_FarClip);
	/*m_Projection = glm::scale(m_Projection, { -1,1,-1 });*/
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
	auto input = Input::GetInstance();
	auto const lookSensitivity = m_LookSensitivity * timeStep.GetSeconds();
	auto const panSensitivity = m_PanSensitivity * timeStep.GetSeconds();
	
	if (input->GetKey(SDL_SCANCODE_LALT))
	{
		//Yaw and Pitch
		if (input->GetMouseButton(Input::MouseButton::left))
		{
			m_Yaw += input->GetMouseX() * lookSensitivity;
			m_Pitch += input->GetMouseY() * lookSensitivity;
		}
		//Zoom
		if (input->GetMouseButton(Input::MouseButton::right))
		{
			m_Position.z += input->GetMouseX() * m_ZoomSensitivity * timeStep.GetSeconds();
		}
	}

	//Pan
	if (input->GetMouseButton(Input::MouseButton::middle))
	{
		m_Position.x += input->GetMouseX() * panSensitivity;
		m_Position.y -= input->GetMouseY() * panSensitivity;
	}

	//Zoom
	if (input->getWheelY() > 0)
	{
		m_Position.z += 1.0f * m_ZoomSensitivity;
	}
	if (input->getWheelY() < 0)
	{
		m_Position.z -= 1.0f * m_ZoomSensitivity;
	}
	
	if (input->GetKeyDown(SDL_SCANCODE_X))
	{
		m_Yaw = -90.0f;
		m_Pitch = 0.0f;
	}
	
	UpdateView();
}
