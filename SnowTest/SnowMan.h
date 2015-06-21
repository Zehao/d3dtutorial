#ifndef __SNOWMAN_H__
#define __SNOWMAN_H__
#include "XFileObject.h"


class SnowMan:public XFileObject
{

public:
	SnowMan(IDirect3DDevice9 *device, Terrain *terr, string path);
	virtual ~SnowMan();

public:
	 virtual void draw(float x, float z);

	 void draw(float x, float z, float rotationAngle);
};


#endif