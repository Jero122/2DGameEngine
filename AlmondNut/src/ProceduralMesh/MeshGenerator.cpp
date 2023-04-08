#include "MeshGenerator.h"

MeshData MeshGenerator::GeneratePlane(float width, float height, unsigned widthSegments, unsigned heightSegments)
{
    MeshData meshData;

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float segmentWidth = width / static_cast<float>(widthSegments);
    float segmentHeight = height / static_cast<float>(heightSegments);

    unsigned int vertexIndex = 0;

    for (unsigned int i = 0; i <= heightSegments; ++i) {
        for (unsigned int j = 0; j <= widthSegments; ++j) {
            float x = j * segmentWidth - halfWidth;
            float y = i * segmentHeight - halfHeight;
            float u = static_cast<float>(j) / static_cast<float>(widthSegments);
            float v = static_cast<float>(i) / static_cast<float>(heightSegments);

            meshData.positions.push_back(glm::vec3(x, 0, y));
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
