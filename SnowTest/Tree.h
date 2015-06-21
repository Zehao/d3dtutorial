#ifndef __TREE_H__
#define __TREE_H__

#include "XFileObject.h"

class Tree : public XFileObject{

public:
	Tree(IDirect3DDevice9 *device, Terrain *terr, string path);
	void draw(float x, float z);
	void draw(float x, float z, float scale);

};

#endif