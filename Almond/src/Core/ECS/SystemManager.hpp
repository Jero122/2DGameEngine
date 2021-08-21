#pragma once
#include <unordered_map>
#include <memory>
#include <cassert>
#include "ECSTypes.hpp"

class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> CreateSystem()
	{
		const char* systemName = typeid(T).name();
		assert(m_SystemNameToIndex.find(systemName) == m_SystemNameToIndex.end() && "System already created");
		
		auto system = std::make_shared<T>();
		m_Systems.push_back(system);
		return system;
	}

	template<typename T>
	std::shared_ptr<T> CreateManualSystem()
	{
		const char* systemName = typeid(T).name();
		assert(m_ManualSystemNameToIndex.find(systemName) == m_ManualSystemNameToIndex.end() && "System already created");

		auto system = std::make_shared<T>();
		m_ManualSystems.push_back(system);
		return system;
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		const char* systemName = typeid(T).name();
		assert(m_SystemNameToIndex.find(systemName) == m_SystemNameToIndex.end() && "System used before creation");
		assert(m_ManualSystemNameToIndex.find(systemName) == m_ManualSystemNameToIndex.end() && "System used before creation");
		m_Signatures.insert({ systemName, signature });
	}
	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (auto const& system : m_Systems)
		{
			const char* type = typeid(system).name();
			auto const& systemSignature = m_Signatures[type];

			//if entity and system signatures match - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
				system->EntityAdded(entity);
			}
			//if signature does not match, try to erase that entity (if it exists)
			else
			{
				system->mEntities.erase(entity);
				system->EntityRemoved(entity);
			}
		}

		for (auto const& system : m_ManualSystems)
		{
			const char* type = typeid(system).name();
			auto const& systemSignature = m_Signatures[type];

			//if entity and system signatures match - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
				system->EntityAdded(entity);
			}
			//if signature does not match, try to erase that entity (if it exists)
			else
			{
				system->mEntities.erase(entity);
				system->EntityRemoved(entity);
			}
		}
	}
	void EntityDestroyed(Entity entity)
	{
		for (auto const& system: m_Systems)
		{
			system->EntityRemoved(entity);
			system->mEntities.erase(entity);

		}

		for (auto const& system : m_ManualSystems)
		{
			system->EntityRemoved(entity);
			system->mEntities.erase(entity);

		}
	}

	//map from system type string pointer to system
	std::vector<std::shared_ptr<System>> m_Systems;
	std::vector<std::shared_ptr<System>> m_ManualSystems;

	
	
private:
	//map from system type string pointer to its signature
	std::unordered_map<const char*, Signature> m_Signatures;
	
	//map from system type string pointer to it's index in systems vector
	std::unordered_map<const char*, int> m_SystemNameToIndex;
	//map from system type string pointer to it's index in manual systems vector
	std::unordered_map<const char*, int> m_ManualSystemNameToIndex;
};
