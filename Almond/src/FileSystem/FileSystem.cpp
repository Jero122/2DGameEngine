#include "FileSystem.h"

#include "Core/Input.h"
#include "efsw/efsw.hpp"
#include "Core/Log.h"



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
		AddNode(RootNode, dir_entry);
	}

	CurrentNode = RootNode;
}

void FileSystem::OnEnd()
{
	
}

void FileSystem::OnUpdate(TimeStep timeStep)
{

}

void FileSystem::OnImGuiRender()
{
	
}

void FileSystem::OnLateUpdate()
{
	if (dirty)
	{
		ReconstructFileTree();
		dirty = false;
	}
}

void FileSystem::ReconstructFileTree()
{
	//TODO deleting alot of files in debug mode can cause a crash

	//Make sure the current node still exists
	auto CurrentNodeCached = SearchString(CurrentNode->fileName, RootNode);
	//If Current node doesn't exist, try to to find a parent
	if (!CurrentNodeCached)
	{
		auto ParentPath = CurrentNode->dir_entry.path();
		while (!CurrentNodeCached)
		{
			ParentPath = ParentPath.parent_path();
			CurrentNodeCached = FileSystem::SearchString(ParentPath.filename().string(), RootNode);
		}
	}

	//Clear the file tree
	RootNode->childNodes = {};

	//Regenerate tree
	for (auto& dir_entry : std::filesystem::directory_iterator(s_AssetsDirectory))
	{
		AddNode(RootNode, dir_entry);
	}

	//Restore current node
}

void FileSystem::AddNode(std::shared_ptr<FileNode> parentNode, std::filesystem::directory_entry const& dir_entry)
{
	auto node = std::make_shared<FileNode>(dir_entry);

	for (auto const& rec_dir_entry : std::filesystem::directory_iterator(dir_entry.path()))
	{
		if (rec_dir_entry.is_directory())
		{
			AddNode(node, rec_dir_entry);
		}
		else
		{
			node->childNodes.push_back(std::make_shared<FileNode>(rec_dir_entry));
		}
	}

	parentNode->childNodes.push_back(node);
}

std::vector<std::shared_ptr<FileSystem::FileNode>> FileSystem::SearchSubString(const std::string subString,
	std::shared_ptr<FileNode> searchRoot)
{
	std::vector<std::shared_ptr<FileNode>> result;

	for (auto child_node : searchRoot->childNodes)
	{
		auto search1 = std::search(
			child_node->fileName.begin(), child_node->fileName.end(),
			subString.begin(), subString.end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);

		auto search2 = std::search(
			subString.begin(), subString.end(),
			child_node->fileName.begin(), child_node->fileName.end(),
			[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
		);

		if (search1 != child_node->fileName.end() || search2 != subString.end())
		{
			result.push_back(child_node);
		}

		if (child_node->dir_entry.is_directory())
		{
			auto recursiveSearch = SearchSubString(subString, child_node);
			result.insert(std::end(result), recursiveSearch.begin(), recursiveSearch.end());
		}
	}

	return result;
}

std::shared_ptr<FileSystem::FileNode> FileSystem::SearchString(const std::string string,
	std::shared_ptr<FileNode> searchRoot)
{
	if (searchRoot->fileName == string)
	{
		return searchRoot;
	}

	for (auto child_node : searchRoot->childNodes)
	{
		if (child_node->dir_entry.is_directory())
		{
			auto find = SearchString(string, child_node);
			if (find)
			{
				return find;
			}
		}
 	}
	return nullptr;
}

