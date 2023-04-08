#pragma once
#include <memory>
#include "Renderer/Mesh/ProceduralMesh.h"

struct ProceduralMeshComponent
{
	std::shared_ptr<ProceduralMesh> mesh;

	explicit ProceduralMeshComponent(const std::shared_ptr<ProceduralMesh>& mesh)
		: mesh(mesh)
	{
	}

	ProceduralMeshComponent() = default;
};
