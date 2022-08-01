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
		AL_ENGINE_WARN("Mesh File: {0} Not Found, Generating...", meshPath);
		//Read FBX/OBJ and save into mesh file

		const unsigned int flags = 0 |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_LimitBoneWeights |
			aiProcess_SplitLargeMeshes |
			aiProcess_ImproveCacheLocality |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_FindDegenerates |
			aiProcess_FindInvalidData |
			aiProcess_GenUVCoords;

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path.c_str(), flags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			AL_ENGINE_ERROR("ASSIMP{0}:", import.GetErrorString());
			return;
		}

		//resize meshdata meshes array
		m_MeshData.meshDescriptions.reserve(scene->mNumMeshes);

		//Convert AIMeshes to internal MeshData format
		for (unsigned int i = 0; i != scene->mNumMeshes; i++)
		{
			AL_ENGINE_INFO("Converting mesh: {0} ", scene->mMeshes[i]->mName.C_Str());
			m_MeshData.meshDescriptions.push_back(ConvertAIMesh(scene->mMeshes[i]));
		}

		//Save mesh data
		AL_ENGINE_INFO("Writing .mesh file for: {0} ", rawname);
		SaveMeshFile(meshPath.c_str());
	}
}

MeshDescription AMesh::ConvertAIMesh(const aiMesh* m)
{
	const bool hasTexCoords = m->HasTextureCoords(0);
	const uint32_t numIndices = m->mNumFaces * 3;

	const uint32_t streamElementSize = numElementsToStore * sizeof(float);

	for (int i = 0; i != m->mNumVertices; ++i)
	{
		const aiVector3D v = m->mVertices[i];
		const aiVector3D n = m->mNormals[i];
		const aiVector3D t = hasTexCoords ? m->mTextureCoords[0][i] : aiVector3D();

		m_MeshData.vertexData.push_back(v.x);
		m_MeshData.vertexData.push_back(v.y);
		m_MeshData.vertexData.push_back(v.z);

		m_MeshData.vertexData.push_back(n.x);
		m_MeshData.vertexData.push_back(n.y);
		m_MeshData.vertexData.push_back(n.z);

		m_MeshData.vertexData.push_back(t.x);
		m_MeshData.vertexData.push_back(t.y);
	}

	for (int i = 0; i != m->mNumFaces; ++i)
	{
		const aiFace& F = m->mFaces[i];

		m_MeshData.indexData.push_back(F.mIndices[0] + m_VertexOffset);
		m_MeshData.indexData.push_back(F.mIndices[1] + m_VertexOffset);
		m_MeshData.indexData.push_back(F.mIndices[2] + m_VertexOffset);
	}

	MeshDescription result = MeshDescription();
	
	result.lodCount = 1;
	result.streamCount = 1;
	result.indexOffset = m_IndexOffset;
	result.vertexOffset = m_VertexOffset;
	result.vertexCount = m->mNumVertices;
	result.lodOffset[0] = m_IndexOffset * sizeof(uint32_t);
	result.lodOffset[1] = (m_IndexOffset + numIndices) * sizeof(uint32_t);
	result.streamOffset[0] = m_VertexOffset * streamElementSize;
	result.streamElementSize[0] = streamElementSize;


	m_IndexOffset += numIndices;
	m_VertexOffset += m->mNumVertices;
	return result;
}

void AMesh::SaveMeshFile(const char* filename)
{
	FILE* f = fopen(filename, "wb");

	uint32_t magicValue = 0x12345678;
	uint32_t meshCount = (uint32_t)m_MeshData.meshDescriptions.size();
	uint32_t dataBlockOffset = (uint32_t)(sizeof(MeshFileHeader) + meshCount * sizeof(MeshDescription));
	uint32_t indexDataSize = m_MeshData.indexData.size() * sizeof(uint32_t);
	uint32_t vertexDataSize = m_MeshData.vertexData.size() * sizeof(float);

	const MeshFileHeader header = {
		magicValue, meshCount,dataBlockOffset,indexDataSize,vertexDataSize
	};

	//save header
	fwrite(&header, 1, sizeof(header), f);

	//save mesh descriptions
	fwrite(m_MeshData.meshDescriptions.data(), header.meshCount, sizeof(MeshDescription), f);
	//save index data
	fwrite(m_MeshData.indexData.data(), 1, header.indexDataSize, f);
	//save vertex data
	fwrite(m_MeshData.vertexData.data(), 1, header.vertexDataSize, f);

	fclose(f);
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

	if (fread(m_MeshData.meshDescriptions.data(), sizeof(MeshDescription), m_Header.meshCount, f) != m_Header.meshCount)
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
