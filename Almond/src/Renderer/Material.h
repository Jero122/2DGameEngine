#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Texture.h"

struct Material
{
	glm::vec4 albedoColour = { 0.0f,0.0f,0.0f, 0.0f};
	glm::vec4 emissiveColour = { 1.0f, 1.0f, 0.0f ,0.0f };
	//Shininess
	float metallic = 0.0f;
	float roughness = 1.0f;

	float transparencyFactor = 1.0f;
	float alphaTest = 0.0f;

	//Maps
	std::shared_ptr<Texture> albedoMap;
	std::shared_ptr<Texture> aoRoughnessMetallicMap;
	std::shared_ptr<Texture> normalMap;
	std::shared_ptr<Texture> emissiveMap;
};
