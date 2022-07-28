#pragma once

#include "AlmondApplication.h"
#include "Log.h"


#ifdef ALM_PLATFORM_WINDOWS

extern AlmondApplication* CreateApplication();

#undef main
int main(int argc, char* argv[])
{
	Log::Init();
	AL_ENGINE_TRACE("Initliased Log!");

	auto app = CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#endif
