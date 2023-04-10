#include "MeshGenerator.h"
#include "FastNoiseLite/FastNoiseLite/FastNoiseLite.h"
#include <array>

MeshData MeshGenerator::GeneratePlane(int width, int height, unsigned widthSegments, unsigned heightSegments)
{
    MeshData meshData;

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float segmentWidth = width / static_cast<float>(widthSegments);
    float segmentHeight = height / static_cast<float>(heightSegments);

    unsigned int vertexIndex = 0;
    std::vector<std::vector<float>> noiseData = GenerateNoise(widthSegments + 1, heightSegments + 1, 0.1f);

    for (unsigned int i = 0; i <= heightSegments; ++i) {
        for (unsigned int j = 0; j <= widthSegments; ++j) {
            float x = j * segmentWidth - halfWidth;
            float y = i * segmentHeight - halfHeight;
            float z = noiseData[i][j] * 2;
            float u = static_cast<float>(j) / static_cast<float>(widthSegments);
            float v = static_cast<float>(i) / static_cast<float>(heightSegments);

            meshData.positions.push_back(glm::vec3(x, z, y));
            meshData.normals.push_back(glm::vec3(0, 1, 0));
            meshData.uvs.push_back(glm::vec2(u, v));

            if (i < heightSegments && j < widthSegments) {
                unsigned int a = vertexIndex;
                unsigned int b = vertexIndex + widthSegments + 1;
                unsigned int c = vertexIndex + 1;
                unsigned int d = vertexIndex + widthSegments + 2;

                // Add two triangles to form a quad
                meshData.indices.push_back(a);
                meshData.indices.push_back(b);
                meshData.indices.push_back(c);

                meshData.indices.push_back(b);
                meshData.indices.push_back(d);
                meshData.indices.push_back(c);
            }

            ++vertexIndex;
        }
    }

    return meshData;
}

std::vector<std::vector<float>> MeshGenerator::GenerateNoise(int width, int height, float frequency)
{
    std::vector<std::vector<float>> noiseData(height, std::vector<float>(width));

    // Create and configure FastNoiseLite object
    fnl_state noise;
    noise.noise_type = FNL_NOISE_PERLIN;

    // Set the frequency
    noise.frequency = frequency;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            noiseData[y][x] = fnlGetNoise2D(&noise, static_cast<float>(x), static_cast<float>(y));
        }
    }

    return noiseData;
}
