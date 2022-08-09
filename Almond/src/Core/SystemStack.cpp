#include "SystemStack.h"

void SystemStack::PushSystem(SubSystem* system)
{
	m_Systems.emplace(m_Systems.begin() + m_SystemInsertIndex, system);
	m_SystemInsertIndex++;
}

void SystemStack::PopSystem(SubSystem* system)
{
	auto it = std::find(m_Systems.begin(), m_Systems.begin() + m_SystemInsertIndex, system);
	if (it != m_Systems.begin() + m_SystemInsertIndex)
	{
		system->OnEnd();
		m_Systems.erase(it);
		m_SystemInsertIndex--;
	}
}


void SystemStack::PushOverLay(SubSystem* overlay)
{
	m_Systems.emplace_back(overlay);
}

void SystemStack::PopOverlay(SubSystem* overlay)
{
	auto it = std::find(m_Systems.begin() + m_SystemInsertIndex, m_Systems.end(), overlay);
	if (it != m_Systems.begin() + m_SystemInsertIndex)
	{
		overlay->OnEnd();
		m_Systems.erase(it);
	}
}


SystemStack::~SystemStack()
{
	for (SubSystem* system : m_Systems)
	{
		system->OnEnd();
		delete system;
	}
}
