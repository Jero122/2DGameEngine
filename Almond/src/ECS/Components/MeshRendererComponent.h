#pragma once
#pragma once
#include <string>

#include "Renderer/GLFramework/GLMesh.h"
#include "Renderer/Mesh/AMesh.h"


struct MeshRendererComponent
{
	std::shared_ptr<AMesh> mesh;
	std::shared_ptr<GLMesh> glMesh;

	explicit MeshRendererComponent(const std::shared_ptr<AMesh>& mesh)
		: mesh(mesh)
	{
		glMesh = std::make_shared<GLMesh>(mesh->GetMeshData().indexData.data(), mesh->GetMeshHeader().indexDataSize, mesh->GetMeshData().vertexData.data(), mesh->GetMeshHeader().vertexDataSize);
	}

	MeshRendererComponent() = default;
};
