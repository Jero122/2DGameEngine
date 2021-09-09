#pragma once
#include "ECS/Components/SpriteRender.h"
#include "ECS/Components/Transform.h"


class Renderer2D
{
public:
	static void Init();
	static void Shutdown();
	static void BeginScene();
	static void EndScene();
	static void Submit(Transform& transform, SpriteRender& spriteRender);

};

