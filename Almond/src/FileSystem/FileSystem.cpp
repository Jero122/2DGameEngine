#include "FileSystem.h"
#include "efsw/efsw.hpp"
#include "Core/Log.h"



void FileSystem::OnStart()
{
	m_FileWatcher = std::make_unique<efsw::FileWatcher>();
	m_Listener = std::make_unique<UpdateListener>();


	efsw::WatchID watchID = m_FileWatcher->addWatch("assets", m_Listener.get(), true);


	// Start watching asynchronously the directories
	m_FileWatcher->watch();
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
	
}

