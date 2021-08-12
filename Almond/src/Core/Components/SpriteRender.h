#pragma once
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

struct SpriteRender
{
	float width;
	float height;
	glm::vec4 color;
	unsigned int textureID;
	glm::vec2 texCoords[4];

	SpriteRender() = default;

	SpriteRender(float width, float height, const glm::vec4& color)
		: width(width),
		  height(height),
		  color(color)
	{
		textureID = 0;
	}

	SpriteRender(float width, float height, unsigned texture_id, glm::vec2 topRight, glm::vec2 bottomRight, glm::vec2 bottomLeft, glm::vec2 topLeft)
		: width(width),
		  height(height),
		  textureID(texture_id)
	{
		texCoords[0] = topRight;
		texCoords[1] = bottomRight;
		texCoords[2] = bottomLeft;
		texCoords[3] = topLeft;
	}

	SpriteRender(float width, float height, unsigned texture_id)
		: width(width),
		  height(height), color(glm::vec4{255,255,255,1}),
		  textureID(texture_id)
	{
		texCoords[0] = glm::vec2(1, 1);
		texCoords[1] = glm::vec2(1, 0);
		texCoords[2] = glm::vec2(0, 0);
		texCoords[3] = glm::vec2(0, 1);
	}
};
