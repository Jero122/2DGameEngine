#pragma once
#include <string>

#include "TimeStep.h"

class SubSystem
{
public:
	SubSystem() = default;
	virtual ~SubSystem() = default;

	/**
	 * \brief Called at the start of the engine initialisation routine to initialise this system
	 */
	virtual void OnStart() {}
	/**
	 * \brief Called at the end of the application runtime, to de initialise this system
	 */
	virtual void OnEnd(){}
	/**
	 * \brief Called once a frame, for application/game logic
	 * \param timeStep the delta between the last frame and the current frame
	 */
	virtual void OnUpdate(TimeStep timeStep){}
	/**
	 * \brief Called once a frame, for ImGui logic.
	 */
	virtual void OnImGuiRender(){}
	/**
	 * \brief Similar to OnUpdate, but called at the end of the runtime loop
	 */
	virtual void OnLateUpdate(){}
};

