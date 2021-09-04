#pragma once
#include <unordered_map>
#include <memory>
#include <cassert>
#include "ECSTypes.h"

class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> CreateSystem()
	{
		const char* systemName = typeid(T).name();
		assert(mSystems.find(systemName) == mSystems.end() && "System already created");
		
		auto system = std::make_shared<T>();
		mSystems.insert({ systemName, system });
		return system;
	}

	template<typename T>
	std::shared_ptr<T> CreateManualSystem()
	{
		const char* systemName = typeid(T).name();
		assert(mManualSystems.find(systemName) == mManualSystems.end() && "System already created");

		auto system = std::make_shared<T>();
		mManualSystems.insert({ systemName, system });
		return system;
	}

private:
	//map from system type string pointer to system
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems;

	//map from system type string pointer to system
	std::unordered_map<const char*, std::shared_ptr<System>> mManualSystems;
};
	