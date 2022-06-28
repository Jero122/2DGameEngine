#include "DebugDrawBox2D.h"
#include <GL/glew.h>

#include "Renderer/GLCall.h"
#include "Renderer/Shader.h"
#include "glm/ext/matrix_clip_space.hpp"


struct GLRenderLines
{
	void Create()
	{
		std::string shaderPath("assets/shaders/DebugLine");
		m_shader = Shader();
		m_shader.init(shaderPath);

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(2, m_VBOs);

		
		glBindVertexArray(m_VAO);
	

		//Vertex Buffer
		//Positions
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);

		//Colors
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[1]);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(1);

		// Cleanup
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		m_count = 0;
	}
	void Destroy()
	{
		
	}

	void AddVertex(const b2Vec2& v, const b2Color& c)
	{
		if (m_count == e_maxVertices)
			Flush();

		m_vertices[m_count] = v;
		m_colors[m_count] = c;
		m_count++;
	}

	void Flush()
	{
		if (m_count == 0)
			return;

		/*m_shader.use();
		Camera camera = Camera();
		camera.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 view = camera.GetViewMatrix();

		m_shader.setMat4("uView", view);
		m_shader.setMat4("uProjection", projection);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);

		GLCALL(glDrawArrays(GL_LINES, 0, m_count));*/

		m_count = 0;
	}

	enum { e_maxVertices = 2 * 512 };
	b2Vec2 m_vertices[e_maxVertices];
	b2Color m_colors[e_maxVertices];
	int32 m_count;

	GLuint m_VAO;
	GLuint m_VBOs[2];
	Shader m_shader;
	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
	
};

DebugDrawBox2D::DebugDrawBox2D()
{
	m_lines = NULL;
}

DebugDrawBox2D::~DebugDrawBox2D()
{
	b2Assert(m_lines == NULL);
}


void DebugDrawBox2D::Create()
{
	m_lines = new GLRenderLines;
	m_lines->Create();
}

void DebugDrawBox2D::Destroy()
{
	m_lines->Destroy();
	delete m_lines;
	m_lines = NULL;
}

void DebugDrawBox2D::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
		b2Vec2 p1 = vertices[vertexCount - 1];
	for (int32 i = 0; i < vertexCount; ++i)
	{
		b2Vec2 p2 = vertices[i];
		m_lines->AddVertex(p1, color);
		m_lines->AddVertex(p2, color);
		p1 = p2;
	}
}

void DebugDrawBox2D::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	b2Color newColour = b2Color(255/255, 0/255, 0/255, 1);
	b2Vec2 p1 = vertices[vertexCount - 1];
	for (int32 i = 0; i < vertexCount; ++i)
	{
		b2Vec2 p2 = vertices[i];
		m_lines->AddVertex(p1, newColour);
		m_lines->AddVertex(p2, newColour);
		p1 = p2;
	}
}

void DebugDrawBox2D::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
}

void DebugDrawBox2D::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
}

void DebugDrawBox2D::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
}

void DebugDrawBox2D::DrawTransform(const b2Transform& xf)
{
}

void DebugDrawBox2D::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	
}


void DebugDrawBox2D::Flush()
{
	m_lines->Flush();
}
