#pragma once
#include <vector>

#include "SubSystem.h"

class SystemStack
{
public:
	SystemStack() = default;
	~SystemStack();

	void PushSystem(SubSystem* system);
	void PopSystem(SubSystem* system);

	void PushOverLay(SubSystem* overlay);
	void PopOverlay(SubSystem* overlay);

	std::vector<SubSystem*>::iterator begin() { return m_Systems.begin(); }
	std::vector<SubSystem*>::iterator end() { return m_Systems.end(); }

	std::vector<SubSystem*>::const_iterator Begin() const { return m_Systems.begin(); }
	std::vector<SubSystem*>::const_iterator End() const { return m_Systems.end(); }

private:
	std::vector<SubSystem*> m_Systems;
	unsigned int m_SystemInsertIndex = 0;
};
