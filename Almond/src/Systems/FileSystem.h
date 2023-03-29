#pragma once
#include <filesystem>
#include <memory>

#include <efsw/efsw.hpp>

#include "Core/Log.h"
#include "Core/SubSystem.h"
#include "Renderer/Mesh/AMesh.h"

/**
 * \brief FileSystem for the Almond Game Engine
 * Listens for file/directory changes using ESFM in the assets folder and maintains a file tree using std::filesystem
 * Automatically converts 3D Models to a custom binary mesh format for faster loading and smaller asset sises
 */
class FileSystem : public SubSystem
{
public:

	class UpdateListener : public efsw::FileWatchListener
	{
	public:
		void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename);
	};

	class FileNode
	{
	public:
		FileNode() = default;
		FileNode(std::filesystem::directory_entry entry)
			:m_DirEntry(entry)
		{
			m_FileName = m_DirEntry.path().filename().string();
		}
		std::filesystem::directory_entry m_DirEntry;
		std::vector<std::shared_ptr<FileNode>> m_ChildNodes;
		std::shared_ptr<FileNode> m_ParentNode = nullptr;
		std::string m_FileName;
		std::string m_IconName;
	};

	inline static const std::filesystem::path s_AssetsDirectory = "assets";
	inline static std::shared_ptr<FileNode> s_RootNode = std::make_shared<FileNode>();
	inline static std::shared_ptr<FileNode> s_CurrentNode;

	FileSystem() = default;
	~FileSystem() = default;

	void OnStart() override;
	void OnLateUpdate() override;

	/**
	 * \brief Performs a recursive bidirectional sub string search in the given directory
	 * \param subString 
	 * \param searchRoot 
	 * \return a vector of FileNodes which have the subString in their name, an empty vector is returned if no matches
	 */
	static std::vector<std::shared_ptr<FileNode>> SearchSubString(const std::string subString, std::shared_ptr<FileNode> searchRoot);

	/**
	 * \brief Performs a recursive search using a vector of substrings in the given directory
	 * \param subStrings 
	 * \param searchRoot 
	 * \return a vector of FileNodes which have any of the subStrings in their name, an empty vector is returned if no matches
	 */
	static std::vector<std::shared_ptr<FileNode>> SearchSubStrings(const std::vector<std::string> subStrings, std::shared_ptr<FileNode> searchRoot);

	/**
	 * \brief Performs an string search in the given directory
	 * \param string 
	 * \param searchRoot 
	 * \return a matching FileNode, returns nullptr if no matches are found
	 */
	static std::shared_ptr<FileNode> SearchString(const std::string string, std::shared_ptr<FileNode> searchRoot);

private:
	inline static bool dirty = false;

	/**
	 * \brief Processes assets such as models and textures, and converts them into custom binary assets for faster loading
	 */
	void ProcessAssets();
	/**
	 * \brief Saves the meshData to file with filename
	 * \param meshData The mesh data to save
	 * \param filename the name and location of the file to save
	 */
	void SaveMeshFile(MeshData meshData, const char* filename);

	/**
	 * \brief Converts an Assimp Mesh into MeshData for serialisation
	 * \param meshData 
	 * \param m
	 * \return 
	 */
	MeshDescription ConvertAIMesh(MeshData& meshData, const aiMesh* m);

	/**
	 * \brief Reconstructs the file tree. Should only be used when the file tree is modified
	 */
	static void ReconstructFileTree();

	/**
	 * \brief Adds a node to the file tree
	 * \param parentNode This is the node to which the new node will be added
	 * \param dir_entry The directory entry of the new node to add
	 */
	static void AddNode(std::shared_ptr<FileNode> parentNode, std::filesystem::directory_entry const& dir_entry);

;
	uint32_t m_IndexOffset = 0;
	uint32_t m_VertexOffset = 0;

	std::unique_ptr<efsw::FileWatcher> m_FileWatcher;
	std::unique_ptr<UpdateListener> m_Listener ;
};

