#include "SpriteSheet.h"

/*SpriteSheet::SpriteSheet(const Texture& m_texture, unsigned SpriteWidth, unsigned SpriteHeight, unsigned numRows,
	unsigned numCols): m_Texture(m_texture), m_SpriteWidth(SpriteWidth), m_SpriteHeight(SpriteHeight), m_NumRows(numRows), m_NumCols(numCols)
{
	
}*/

SpriteSheet::SpriteSheet(const std::string filePath, unsigned SpriteWidth, unsigned SpriteHeight, unsigned numRows,
                         unsigned numCols): m_Texture(Texture(filePath)), m_SpriteWidth(SpriteWidth), m_SpriteHeight(SpriteHeight), m_NumRows(numRows), m_NumCols(numCols)
{
	for (int y = 0; y < numCols; ++y)
	{
		for (int x = 0; x < numRows; ++x)
		{
			// access [x,y] = m_UVQuads[y * m_NumCols + x];
			Quad quad;

			
			quad.topRight = glm::vec2{ (float)((x * SpriteWidth) + SpriteWidth) / (float)m_Texture.GetWidth(),  (float)((y * SpriteHeight) + SpriteHeight) / (float)m_Texture.GetHeight() };
			quad.bottomRight = glm::vec2{ (float)((x * SpriteWidth) + SpriteWidth) / (float)m_Texture.GetWidth(),  (float)(y * SpriteHeight) / (float)m_Texture.GetHeight() };
			quad.bottomLeft = glm::vec2{ (float)(x * SpriteWidth) / (float)m_Texture.GetWidth(),  (float)(y * SpriteHeight) / (float)m_Texture.GetHeight() };
			quad.topLeft = glm::vec2{ (float)(x * SpriteWidth) / (float)m_Texture.GetWidth(),  (float)((y * SpriteHeight) + SpriteHeight) / (float)m_Texture.GetHeight() };
			m_UVQuads[y * m_NumCols + x] = quad;
		}
	}
}

SpriteSheet::~SpriteSheet()
{
	delete[] m_UVQuads;
}