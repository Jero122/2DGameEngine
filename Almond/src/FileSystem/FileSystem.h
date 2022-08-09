#pragma once
#include <iostream>
#include <memory>

#include <efsw/efsw.hpp>
#include "Core/SubSystem.h"

class UpdateListener : public efsw::FileWatchListener
{
public:
	void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename) override
	{
		switch (action)
		{
		case efsw::Actions::Add:
			std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added" << std::endl;
			break;
		case efsw::Actions::Delete:
			std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
			break;
		case efsw::Actions::Modified:
			std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified" << std::endl;
			break;
		case efsw::Actions::Moved:
			std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from (" << oldFilename << ")" << std::endl;
			break;
		default:
			std::cout << "Should never happen!" << std::endl;
		}
	}
};

class FileSystem :
    public SubSystem
{
public:
	FileSystem() = default;
	~FileSystem() = default;

	void OnStart() override;
	void OnEnd() override;
	void OnUpdate(TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnLateUpdate() override;

private:
	std::unique_ptr<efsw::FileWatcher> m_FileWatcher;
	std::unique_ptr<UpdateListener> m_Listener ;
};

