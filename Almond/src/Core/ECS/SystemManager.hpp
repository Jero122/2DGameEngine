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

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		const char* systemName = typeid(T).name();
		

		if (mManualSystems.find(systemName) != mManualSystems.end())
		{
			mSignatures.insert({ systemName, signature });
		}
		else if (mSystems.find(systemName) != mSystems.end())
		{
			mSignatures.insert({ systemName, signature });
		}
		else
		{
			assert(false, "System used before creation");
		}
		/*assert(mSignatures.find(systemName) == mSignatures.end() && "System used before creation");
		mSignatures.insert({ systemName, signature });*/
	}
	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];


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

		for (auto const& pair : mManualSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

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
		for (auto const& pair: mManualSystems)
		{
			auto const& system = pair.second;
			system->EntityRemoved(entity);
			system->mEntities.erase(entity);

		}
		for (auto const& pair : mManualSystems)
		{
			auto const& system = pair.second;
			system->EntityRemoved(entity);
			system->mEntities.erase(entity);

		}
	}

private:
	//map from system type string pointer to its signature
	std::unordered_map<const char*, Signature> mSignatures;
	//map from system type string pointer to system
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems;

	//map from system type string pointer to system
	std::unordered_map<const char*, std::shared_ptr<System>> mManualSystems;
};
	