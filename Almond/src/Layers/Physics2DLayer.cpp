#include "Physics2DLayer.h"

#include "ECS/Components/RigidBody.h"
#include "ECS/Components/Transform.h"
#include "ECS/ECS.hpp"
#include "Physics2D/Physics2DSystem.h"


extern ECS ecs;
std::shared_ptr<Physics2DSystem> physics2D;

Physics2DLayer::Physics2DLayer()
{
	physics2D = ecs.CreateSystem<Physics2DSystem>();
	physics2D->interpolate = true;
	physics2D->Init();
	
}


Physics2DLayer::~Physics2DLayer() = default;

void Physics2DLayer::OnAttach()
{
	
}

void Physics2DLayer::OnDetach()
{
	physics2D->ShutDown();
}

void Physics2DLayer::OnUpdate(TimeStep timeStep)
{
	physics2D->Update();
}

void Physics2DLayer::OnImGuiRender()
{
	
}

void Physics2DLayer::OnLateUpdate()
{
}
