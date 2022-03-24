#include <Almond.h>

#include "EditorSystem.h"
#include "Core/EntryPoint.h"

class AlmondNut : public AlmondApplication
{
public:
	AlmondNut()
	{
		PushSystem(new EditorSystem());
	}

	~AlmondNut()
	{
		
	}

};

AlmondApplication* CreateApplication()
{
	return new AlmondNut();
}
