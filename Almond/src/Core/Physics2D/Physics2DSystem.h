#pragma once
#include "Core/ECS/ECSTypes.hpp"

class Physics2DSystem :  public System
{
public:
	void Init() override;
	void Update() override;
	void ShutDown() override;
	void EntityAdded(Entity entity) override;
	void EntityRemoved(Entity entity) override;
};
