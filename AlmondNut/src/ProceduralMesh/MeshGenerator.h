#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct MeshData
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;
};

class MeshGenerator
{

public:
	static MeshData GeneratePlane(float width, float height, unsigned int segmentWidth, unsigned int segmentHeight);
};

