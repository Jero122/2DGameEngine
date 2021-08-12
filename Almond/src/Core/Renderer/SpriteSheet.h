#pragma once
#include "Texture.h"
#include "glm/vec2.hpp"

struct Quad
{
	glm::vec2 topRight;
	glm::vec2 bottomRight;
	glm::vec2 bottomLeft;
	glm::vec2 topLeft;
};

class SpriteSheet
{
public:
	//SpriteSheet(const Texture& m_texture, unsigned int SpriteWidth, unsigned int SpriteHeight, unsigned int numRows, unsigned int numCols);
	

	SpriteSheet(const std::string filePath, unsigned int SpriteWidth, unsigned int SpriteHeight, unsigned int numRows, unsigned int numCols);

	~SpriteSheet();

	unsigned int GetTexID() const
	{
		return m_Texture.GetTexID();
	}


	int GetWidth() const
	{
		return m_Texture.GetWidth();
	}

	int GetHeight() const
	{
		return m_Texture.GetHeight();
	}

	Quad GetQuad(int x, int y)
	{
		return m_UVQuads[y * m_NumCols + x];
	}
private:

	
	Texture m_Texture;
	const unsigned int m_SpriteWidth;
	const unsigned int m_SpriteHeight;

	const unsigned int m_NumRows;
	const unsigned int m_NumCols;

	Quad* m_UVQuads = new Quad[m_NumCols * m_NumRows];
};
