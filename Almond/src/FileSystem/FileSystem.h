#pragma once
#include <filesystem>
#include <iostream>
#include <memory>

#include <efsw/efsw.hpp>

#include "Core/Log.h"
#include "Core/SubSystem.h"


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
			:dir_entry(entry)
		{
			fileName = dir_entry.path().filename().string();
		}
		std::filesystem::directory_entry dir_entry;
		std::vector<std::shared_ptr<FileNode>> childNodes;
		std::string fileName;
		std::string icon;
	};

	inline static const std::filesystem::path s_AssetsDirectory = "assets";
	inline static std::shared_ptr<FileNode> RootNode = std::make_shared<FileNode>();
	inline static std::shared_ptr<FileNode> CurrentNode;

	FileSystem() = default;
	~FileSystem() = default;

	void OnStart() override;
	void OnEnd() override;
	void OnUpdate(TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;

	static std::vector<std::shared_ptr<FileNode>> SearchSubString(const std::string subString, std::shared_ptr<FileNode> searchRoot);
	static std::shared_ptr<FileNode> SearchString(const std::string string, std::shared_ptr<FileNode> searchRoot);
private:
	inline static bool dirty = false;

	static void ReconstructFileTree();
	static void AddNode(std::shared_ptr<FileNode> parentNode, std::filesystem::directory_entry const& dir_entry);

	

	std::unique_ptr<efsw::FileWatcher> m_FileWatcher;
	std::unique_ptr<UpdateListener> m_Listener ;
};

