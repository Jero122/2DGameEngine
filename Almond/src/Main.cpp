#include "Core/Application.h"
const float SCR_WIDTH = 1920;
const float SCR_HEIGHT = 1080;


int main(int argc, char* argv[])
{
    Application* application = new Application();
    application->Run();
	return 0;
}
