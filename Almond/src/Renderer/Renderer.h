#pragma once
#pragma once
#include <memory>

#include "OldCamera.h"
#include "EditorCamera.h"
#include "Shader.h"
#include "OpenGLBuffer.h"
#include "OpenGLVertexArray.h"
#include "glm/fwd.hpp"


class Renderer
{
protected:
	std::unique_ptr<OpenGLVertexArray> m_VertexArray;
	std::unique_ptr<OpenGLVertexBuffer> m_VertexBuffer;
	std::unique_ptr<OpenGLIndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;

	struct Quad
	{
		struct Vertex
		{
			glm::vec3 VertexPosition;
			glm::vec3 Position;
			glm::vec2 Scale;
			float rotation;
			unsigned int Color;
			glm::vec2 TexCoord;
			float TexID;
		};
		Vertex topRight;
		Vertex bottomRight;
		Vertex bottomLeft;
		Vertex topLeft;
	};

public:


	virtual ~Renderer() = default;
	Renderer() = default;
	virtual void Shutdown() = 0;

	//TODO remove OldCamera
	virtual void BeginScene(OldCamera& camera) = 0;
	virtual void BeginScene(EditorCamera& camera) = 0;
	virtual void EndScene() = 0;
	virtual void Submit(const glm::vec3 position, float rotation, glm::vec2 scale, glm::vec4 color, int textureID, glm::vec2* texCoords) = 0;

	struct RenderStats
	{
		uint32_t DrawCalls = 0;
		uint32_t QuadCount = 0;
		uint32_t GetTotalVertexCount() { return  QuadCount * 4; }
		uint32_t GetTotalIndexCount() { return  QuadCount * 6; }
	};



	virtual void ResetStats() = 0;
	virtual RenderStats GetStats() = 0;

};

