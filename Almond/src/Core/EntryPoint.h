#pragma once

#include "AlmondApplication.h"


#ifdef ALM_PLATFORM_WINDOWS

extern AlmondApplication* CreateApplication();

#undef main
int main(int argc, char* argv[])
{
	auto app = CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#endif
