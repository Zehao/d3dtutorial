#ifndef __HOUSE_H__
#define __HOUSE_H__
#include "XFileObject.h"

class House : public XFileObject
{
public:
	House(IDirect3DDevice9 *device, Terrain *terr, string path);
	virtual ~House();

public:
	virtual void draw(float x ,float z);
};


#endif