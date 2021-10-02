#include "Almond.h"

class AlmondNut : public AlmondApplication
{
public:
	AlmondNut()
	{
		
	}

	~AlmondNut()
	{
		
	}

};

AlmondApplication* CreateApplication()
{
	return new AlmondNut();
}
