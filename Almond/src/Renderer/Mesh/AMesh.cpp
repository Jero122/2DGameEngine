#include "AMesh.h"

void AMesh::Load(std::string const& path)
{
	size_t lastdot = path.find_last_of(".");
	if (lastdot == std::string::npos)
	{
		//TODO Move loading stuff to filesystem
		AL_ENGINE_ERROR("AMesh: invalid path: {0}", path);
		return;
	}
	auto rawname = path.substr(0, lastdot);

	//TODO Again, move to filesystem
	auto meshPath = rawname + ".mesh";
	std::ifstream f = std::ifstream(meshPath.c_str());
	if (f.good())
	{
		AL_ENGINE_TRACE("Mesh File: {0} Found", meshPath);
		AL_ENGINE_INFO("Loading mesh: {0} ", meshPath.c_str());
		LoadMeshFile(meshPath.c_str());
	}
	else
	{
		AL_ENGINE_ERROR("Mesh File: {0} Not Found", meshPath);
	}
}

void AMesh::LoadMeshFile(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	assert(f); // Did you forget to run "Ch5_Tool05_MeshConvert"?

	if (!f)
	{
		AL_ENGINE_ERROR("Cannot open {0}", filename);
		exit(EXIT_FAILURE);
	}

	if (fread(&m_Header, 1, sizeof(m_Header), f) != sizeof(m_Header))
	{
		AL_ENGINE_ERROR("Unable to read mesh header");;
		exit(EXIT_FAILURE);
	}

	m_MeshData.meshDescriptions.resize(m_Header.meshCount);

	if (fread(m_MeshData.meshDescriptions.data(), sizeof(AMeshDescription), m_Header.meshCount, f) != m_Header.meshCount)
	{
		AL_ENGINE_ERROR("Unable to read mesh descriptors");
		exit(EXIT_FAILURE);
	}

	m_MeshData.indexData.resize(m_Header.indexDataSize / sizeof(uint32_t));
	m_MeshData.vertexData.resize(m_Header.vertexDataSize / sizeof(float));

	if ((fread(m_MeshData.indexData.data(), 1, m_Header.indexDataSize, f) != m_Header.indexDataSize) ||
		(fread(m_MeshData.vertexData.data(), 1, m_Header.vertexDataSize, f) != m_Header.vertexDataSize))
	{
		AL_ENGINE_ERROR("Unable to read index/vertex data");
		exit(255);
	}

	fclose(f);
}
