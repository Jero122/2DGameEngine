#include "Texture.h"

#include <iostream>

#include "stb/stb_image.h"

Texture::Texture(const std::string filePath)
	:m_FilePath(filePath)
{
	m_Texture = std::make_shared<GLTexture>(GL_TEXTURE_2D, filePath.c_str());
	m_Width = m_Texture->GetWidth();
	m_Height = m_Texture->GetHeight();
}
