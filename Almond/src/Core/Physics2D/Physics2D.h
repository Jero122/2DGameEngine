#pragma once
#include "Core/ECS/ECSTypes.hpp"

class Physics2D : public System
{
public:
	Physics2D() = default;
	void Init() override;
	void Update() override;
	void ShutDown() override;
	void EntityAdded(Entity entity) override;
	void EntityRemoved(Entity entity) override;
};
