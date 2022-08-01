#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include "Core/Log.h"

#include <fstream>
#include <assimp/Importer.hpp>


constexpr const uint32_t kMaxLODs = 8;
constexpr const uint32_t kMaxStreams = 8;
constexpr uint32_t numElementsToStore = 3 + 3 + 2;

struct MeshDescription
{
	//Number of LODs in this mesh, Strictly less than MAX_LODS, last LOD offset is used as a marker only
	uint32_t lodCount = 1;

	//Number of vertex data streams
	uint32_t streamCount = 0;

	//total count of all previous vertices in this mesh file
	uint32_t indexOffset = 0;
	uint32_t vertexOffset = 0;

	//Vertex count, for all LODs
	uint32_t vertexCount = 0;

	//Offsets to LOD data. Last offset is used as a marker to calculate size
	uint32_t lodOffset[kMaxLODs] = { 0 };

	inline uint32_t getLODIndicesCount(uint32_t lod) const { return lodOffset[lod - 1] - lodOffset[lod]; }

	// All the data "pointers" for all the streams
	uint32_t streamOffset[kMaxStreams] = { 0 };

	/* Information about stream element (size pretty much defines everything else, the "semantics" is defined by the shader) */
	uint32_t streamElementSize[kMaxStreams] = { 0 };
};

struct MeshFileHeader
{
	//unique value to check integrity of this file
	uint32_t magicValue;

	//Number of mesh descriptors following this header
	uint32_t meshCount;

	//The offset to combined mesh data, this is the base from which the offsets in individual meshes start
	uint32_t dataBlockStartOffset;

	//How much space index data takes
	uint32_t indexDataSize;

	//How much space vertex data takes
	uint32_t vertexDataSize;
};

struct MeshData
{
	std::vector<MeshDescription> meshDescriptions;
	std::vector<uint32_t> indexData;
	std::vector<float> vertexData;
};

class AMesh
{
public:
	AMesh() = default;

	AMesh(std::string const& path)
	{
		Load(path);
	}

	MeshFileHeader GetMeshHeader() const
	{
		return m_Header;
	}

	MeshData GetMeshData() const
	{
		return m_MeshData;
	}

private:
	

	uint32_t m_IndexOffset = 0;
	uint32_t m_VertexOffset = 0;
	MeshFileHeader m_Header;
	MeshData m_MeshData;

	//Load FBX/ OBJ model
	//Look for a .mesh file first, if exists, read
	//Else load FBX/OBJ, save .mesh file
	void Load(std::string const& path);
	void SaveMeshFile(const char* filename);
	void LoadMeshFile(const char* filename);
	MeshDescription ConvertAIMesh(const aiMesh* m);

};