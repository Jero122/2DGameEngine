#include "FileSystem.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Core/Input.h"
#include "efsw/efsw.hpp"
#include "Core/Log.h"

void FileSystem::UpdateListener::handleFileAction(efsw::WatchID watchid, const std::string& dir,
	const std::string& filename, efsw::Action action, std::string oldFilename)
{
	switch (action)
	{
	case efsw::Actions::Add:
		AL_ENGINE_TRACE("DIR ({0}): FILE ({1}) has been added", dir, filename);
		break;
	case efsw::Actions::Delete:
		AL_ENGINE_TRACE("DIR ({0}): FILE ({1}) has been deleted", dir, filename);
		break;
	case efsw::Actions::Modified:
		AL_ENGINE_TRACE("DIR ({0}): FILE ({1}) has been modified", dir, filename);
		break;
	case efsw::Actions::Moved:
		AL_ENGINE_TRACE("DIR ({0}): FILE ({1}) has been moved from ({2})", dir, filename, oldFilename);
		break;
	default:
		AL_ENGINE_ERROR("Unexpected handleFileAction");
	}
	dirty = true;
}


void FileSystem::ProcessAssets()
{
	//TODO parallelize texture and model conversions
	//TODO TEXTURE CONVERSION
	//TODO GENERATE META FILES FOR ASSETS
	std::vector<std::shared_ptr<FileNode>> textures = SearchSubStrings({ ".png", ".jpg", ".jpeg",  ".tga", ".bmp" }, s_RootNode);
	AL_ENGINE_INFO("FileSystem: Found {0} textures", textures.size());
	//MODEL CONVERSION
	std::vector<std::shared_ptr<FileNode>> models = SearchSubStrings({ ".obj", ".fbx" }, s_RootNode);
	AL_ENGINE_INFO("FileSystem: Found {0} models", models.size());

	for (auto model : models)
	{
		std::string modelPath = model->m_DirEntry.path().generic_string();
		std::string filename = model->m_DirEntry.path().filename().string();

		auto meshName = filename.substr(0, filename.find_last_of(".")) + ".mesh";
		auto meshPath = modelPath.substr(0, modelPath.find_last_of(".")) + ".mesh";

		AL_ENGINE_TRACE("Searching for mesh: {0} in {1}", meshName, model->m_ParentNode->m_DirEntry.path().string());

		//Check if mesh file exists or if the model file was modified
		std::shared_ptr<FileNode> mesh = SearchString(meshName, model->m_ParentNode);
		bool modelModified = false;
		if (mesh)
		{
			//Compares the last write times between mesh and model
			std::filesystem::file_time_type meshLastModified = mesh->m_DirEntry.last_write_time();
			std::filesystem::file_time_type modelLastModified = model->m_DirEntry.last_write_time();
			if(modelLastModified > meshLastModified)
			{
				modelModified = true;
			}
		}

		if (!mesh || modelModified)
		{
			!mesh ? AL_ENGINE_WARN("Mesh File: {0} Not Found, Generating...", meshName) : AL_ENGINE_WARN("Model File Modifed: {0}, Regenerating...", filename);

			//Load model with assimp
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

			const aiScene* scene = aiImportFile(modelPath.c_str(), flags);

			if (!scene || !scene->HasMeshes()) {

				printf("Unable to load '%s'\n", modelPath.c_str());

				return;

			}

			AMeshData mesh_data;
			//resize meshdata meshes array
			mesh_data.meshDescriptions.reserve(scene->mNumMeshes);

			//Convert AIMeshes to internal MeshData format
			for (unsigned int i = 0; i != scene->mNumMeshes; i++)
			{
				AL_ENGINE_TRACE("Converting mesh: {0} ", scene->mMeshes[i]->mName.C_Str());
				mesh_data.meshDescriptions.push_back(ConvertAIMesh(mesh_data, scene->mMeshes[i]));
			}

			//Save mesh data
			AL_ENGINE_TRACE("Writing .mesh file for: {0} ", meshName);
			SaveMeshFile(mesh_data , meshPath.c_str());

			//Reset index/vertex offset
			m_IndexOffset = 0;
			m_VertexOffset = 0;
		}
		else
		{
			AL_ENGINE_INFO("Mesh File: {0} Found", meshName);
		}
	}
}

void FileSystem::SaveMeshFile(AMeshData meshData, const char* filename)
{
	FILE* f = fopen(filename, "wb");

	uint32_t magicValue = 0x12345678;
	uint32_t meshCount = (uint32_t)meshData.meshDescriptions.size();
	uint32_t dataBlockOffset = (uint32_t)(sizeof(AMeshFileHeader) + meshCount * sizeof(AMeshDescription));
	uint32_t indexDataSize = meshData.indexData.size() * sizeof(uint32_t);
	uint32_t vertexDataSize = meshData.vertexData.size() * sizeof(float);

	const AMeshFileHeader header = {
		magicValue, meshCount,dataBlockOffset,indexDataSize,vertexDataSize
	};

	//save header
	fwrite(&header, 1, sizeof(header), f);
	//save mesh descriptions
	fwrite(meshData.meshDescriptions.data(), header.meshCount, sizeof(AMeshDescription), f);
	//save index data
	fwrite(meshData.indexData.data(), 1, header.indexDataSize, f);
	//save vertex data
	fwrite(meshData.vertexData.data(), 1, header.vertexDataSize, f);

	fclose(f);
}

AMeshDescription FileSystem::ConvertAIMesh(AMeshData& meshData, const aiMesh* m)
{
	const bool hasTexCoords = m->HasTextureCoords(0);
	const uint32_t numIndices = m->mNumFaces * 3;

	const uint32_t streamElementSize = numElementsToStore * sizeof(float);

	for (int i = 0; i != m->mNumVertices; ++i)
	{
		const aiVector3D v = m->mVertices[i];
		const aiVector3D n = m->mNormals[i];
		const aiVector3D t = hasTexCoords ? m->mTextureCoords[0][i] : aiVector3D();

		meshData.vertexData.push_back(v.x);
		meshData.vertexData.push_back(v.y);
		meshData.vertexData.push_back(v.z);

		meshData.vertexData.push_back(n.x);
		meshData.vertexData.push_back(n.y);
		meshData.vertexData.push_back(n.z);

		meshData.vertexData.push_back(t.x);
		meshData.vertexData.push_back(t.y);
	}

	for (size_t i = 0; i < m->mNumFaces; i++)
	{
		aiFace face = m->mFaces[i];
		if (face.mNumIndices < 3) {
			continue;
		}
		for (size_t j = 0; j < face.mNumIndices; j++)
			meshData.indexData.push_back(face.mIndices[j] + m_VertexOffset);
	}

	AMeshDescription result = AMeshDescription();

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

void FileSystem::OnStart()
{
	//Create a file watcher
	m_FileWatcher = std::make_unique<efsw::FileWatcher>();
	m_Listener = std::make_unique<UpdateListener>();
	efsw::WatchID watchID = m_FileWatcher->addWatch("assets", m_Listener.get(), true);
	m_FileWatcher->watch();

	//Use std::filesystem to read assets into file tree
	for (auto& dir_entry : std::filesystem::directory_iterator(s_AssetsDirectory))
	{
		AddNode(s_RootNode, dir_entry);
	}
	s_CurrentNode = s_RootNode;

	//Asset conversion
	ProcessAssets();
}

void FileSystem::OnLateUpdate()
{
	if (dirty)
	{
		ReconstructFileTree();
		//Asset conversion
		ProcessAssets();
		//TODO Asset Cleanup
		//delete any meta file for objects that dont exist
		dirty = false;
	}
}

void FileSystem::ReconstructFileTree()
{
	//TODO fix deleting a lot of files in debug mode can cause a crash

	//Make sure the current node still exists
	auto CurrentNodeCached = SearchString(s_CurrentNode->m_FileName, s_RootNode);
	//If Current node doesn't exist, try to to find a parent
	if (!CurrentNodeCached)
	{
		auto ParentPath = s_CurrentNode->m_DirEntry.path();
		while (!CurrentNodeCached)
		{
			ParentPath = ParentPath.parent_path();
			CurrentNodeCached = FileSystem::SearchString(ParentPath.filename().string(), s_RootNode);
		}
	}

	//Clear the file tree
	s_RootNode->m_ChildNodes = {};
	s_CurrentNode = nullptr;

	//Regenerate tree
	for (auto& dir_entry : std::filesystem::directory_iterator(s_AssetsDirectory))
	{
		AddNode(s_RootNode, dir_entry);
	}

	//Restore current node
	s_CurrentNode = CurrentNodeCached;
}

void FileSystem::AddNode(std::shared_ptr<FileNode> parentNode, std::filesystem::directory_entry const& dir_entry)
{
	auto node = std::make_shared<FileNode>(dir_entry);

	//If the node to be added is a directory as well, add it's child nodes to the file tree as well
	if (dir_entry.is_directory())
	{
		for (auto const& rec_dir_entry : std::filesystem::directory_iterator(dir_entry.path()))
		{
			//If the node to be added has a directory child node, recursively add that directory and it's child nodes aswell
			if (rec_dir_entry.is_directory())
			{
				AddNode(node, rec_dir_entry);
			}
			else
			{
				auto childNode = std::make_shared<FileNode>(rec_dir_entry);
				childNode->m_ParentNode = node;
				node->m_ChildNodes.push_back(childNode);
			}
		}
	}
	node->m_ParentNode = parentNode;
	parentNode->m_ChildNodes.push_back(node);
}

std::vector<std::shared_ptr<FileSystem::FileNode>> FileSystem::SearchSubString(const std::string subString,
	std::shared_ptr<FileNode> searchRoot)
{
	std::vector<std::shared_ptr<FileNode>> result;

	for (auto child_node : searchRoot->m_ChildNodes)
	{
		auto search1 = std::search(
			child_node->m_FileName.begin(), child_node->m_FileName.end(),
			subString.begin(), subString.end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);

		auto search2 = std::search(
			subString.begin(), subString.end(),
			child_node->m_FileName.begin(), child_node->m_FileName.end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);

		if (search1 != child_node->m_FileName.end() || search2 != subString.end())
		{
			result.push_back(child_node);
		}

		if (child_node->m_DirEntry.is_directory())
		{
			auto recursiveSearch = SearchSubString(subString, child_node);
			result.insert(std::end(result), recursiveSearch.begin(), recursiveSearch.end());
		}
	}

	return result;
}


std::vector<std::shared_ptr<FileSystem::FileNode>> FileSystem::SearchSubStrings(
	const std::vector<std::string> subStrings, std::shared_ptr<FileNode> searchRoot)
{
	std::vector<std::shared_ptr<FileNode>> result;
	//Search for each substring individually and append the results together
	for (auto subString : subStrings)
	{
		std::vector<std::shared_ptr<FileNode>>subStringSearch = SearchSubString(subString, searchRoot);
		result.insert(result.end(), subStringSearch.begin(), subStringSearch.end());
	}
	return result;
}

std::shared_ptr<FileSystem::FileNode> FileSystem::SearchString(const std::string string,
                                                               std::shared_ptr<FileNode> searchRoot)
{
	//Return if search root is the node being searched for
	if (searchRoot->m_FileName == string)
	{
		return searchRoot;
	}

	//Iterate through search node's children and recursively search them as well
	for (auto child_node : searchRoot->m_ChildNodes)
	{
		//If the child node is a directory, search it as well
		if (child_node->m_DirEntry.is_directory())
		{
			auto find = SearchString(string, child_node);
			if (find)
			{
				return find;
			}
		}
		else if (child_node->m_FileName == string)
		{
			return child_node;
		}
 	}
	return nullptr;
}


