#pragma once
#include "glm/vec4.hpp"

struct SpriteRender
{
	float width;
	float height;
	glm::vec4 colour;
	uint32_t batchID;
};
