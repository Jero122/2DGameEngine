#include <Almond.h>

#include "EditorLayer.h"
#include "Core/EntryPoint.h"

class AlmondNut : public AlmondApplication
{
public:
	AlmondNut()
	{
		PushLayer(new EditorLayer());
	}

	~AlmondNut()
	{
		
	}

};

AlmondApplication* CreateApplication()
{
	return new AlmondNut();
}
