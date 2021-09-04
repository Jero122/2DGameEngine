#pragma once
#include "EntitiyManager.hpp"

template<typename... ComponentTypes>
struct SceneView
{
	SceneView(EntityManager& entityManager) : entityManager(&entityManager)
	{
		if (sizeof...(ComponentTypes) == 0)
		{
			all = true;
		}
		else
		{
			//Unpack template parameters
			int componentIds[] = { 0, GetId<ComponentTypes>() ... };
			for (int i = 0; i < (sizeof...(ComponentTypes) +1); ++i)
			{
				signature.set(componentIds[i]);
			}
		}
	}

	struct Iterator
	{
		Iterator(EntityManager* entityManager, EntityIndex index, Signature signature, bool all)
			:index(index), entityManager(entityManager), signature(signature), all(all)
		{
			
		}

		EntityID operator*() const
		{
			// give back the entityID we're currently at
			return entityManager->entities[index].id;
		}

		bool operator==(const Iterator& other) const
		{
			return index == other.index || index == entityManager->entities.size();
		}

		bool operator!=(const Iterator& other) const
		{
			return index != other.index && index != entityManager->entities.size();
		}

		bool ValidIndex()
		{
			return
				//It is a valid entity ID
				isEntityValid(entityManager->entities[index].id) &&
				//It has the correct component mask
				(all || signature == (signature & entityManager->entities[index].signature));
		}
		
		Iterator& operator++()
		{
			do
			{
				index++;
			}
			while (index < entityManager->entities.size() && !ValidIndex());
			return *this;
		}


		EntityIndex index;
		EntityManager* entityManager;
		Signature signature;
		bool all{ false };
		
	};

	const Iterator begin() const
	{
		int firstIndex = 0;
		while (firstIndex < entityManager->entities.size() &&
			(signature != (signature & entityManager->entities[firstIndex].signature)
				|| !isEntityValid(entityManager->entities[firstIndex].id)))
		{
			firstIndex++;
		}
		return Iterator(entityManager, firstIndex, signature, all);
	}

	const Iterator end() const
	{
		// Give an iterator to the end of this view
		return Iterator(entityManager, EntityIndex(entityManager->entities.size()), signature, all);
	}

	EntityManager* entityManager{nullptr};
	Signature signature;
	bool all{ false };
	
};
