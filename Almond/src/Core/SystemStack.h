#pragma once
#include <vector>

#include "GameSystem.h"

class SystemStack
{
public:
	SystemStack() = default;
	~SystemStack();

	void PushSystem(GameSystem* system);
	void PopSystem(GameSystem* system);

	void PushOverLay(GameSystem* overlay);
	void PopOverlay(GameSystem* overlay);

	std::vector<GameSystem*>::iterator begin() { return m_Systems.begin(); }
	std::vector<GameSystem*>::iterator end() { return m_Systems.end(); }

	std::vector<GameSystem*>::const_iterator Begin() const { return m_Systems.begin(); }
	std::vector<GameSystem*>::const_iterator End() const { return m_Systems.end(); }

private:
	std::vector<GameSystem*> m_Systems;
	unsigned int m_SystemInsertIndex = 0;
};
