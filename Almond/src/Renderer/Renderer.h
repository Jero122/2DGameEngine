#pragma once
#include <memory>
#include "EditorCamera.h"
#include "Shader.h"
#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"
#include "glm/fwd.hpp"
#include "Model.h"

class Renderer
{
protected:
	std::unique_ptr<OpenGLVertexArray> m_VertexArray;
	std::unique_ptr<OpenGLVertexBuffer> m_VertexBuffer;
	std::unique_ptr<OpenGLIndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	glm::vec3 m_ViewPosition;
	float fov;
public:
	virtual ~Renderer() = default;
	Renderer() = default;
	virtual void Shutdown() = 0;
	virtual void BeginScene(EditorCamera & camera) = 0;
	virtual void EndScene() = 0;
	


	struct RenderStats
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t GetTotalVertexCount() { return  QuadCount * 4; }
		uint32_t GetTotalIndexCount() { return  QuadCount * 6; }
	};


	RenderStats m_RenderStats;
	virtual void ResetStats() = 0;
	virtual RenderStats GetStats() = 0;
};

