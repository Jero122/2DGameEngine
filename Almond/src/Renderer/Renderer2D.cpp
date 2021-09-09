#include <GL/glew.h>

#include "Renderer2D.h"
#include "RenderBatch.h"

void Renderer2D::BeginScene()
{
	//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	RenderBatch::BeginBatch();
}

void Renderer2D::EndScene()
{
	RenderBatch::EndBatch();
	RenderBatch::Flush();
}

void Renderer2D::Submit(Transform& transform, SpriteRender& spriteRender)
{
	RenderBatch::Submit(transform, spriteRender);
}


void Renderer2D::Init()
{
	RenderBatch::Init();
}


void Renderer2D::Shutdown()
{
	RenderBatch::Shutdown();
}
