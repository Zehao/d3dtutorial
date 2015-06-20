#ifndef __TREE_H__
#define __TREE_H__

#include "XFileObject.h"

class Tree : public XFileObject{
private:
	D3DLIGHT9 *light;
public:
	Tree(IDirect3DDevice9 *device, Terrain *terr, string path);
	void draw(float x, float z);
	void draw(float x, float z, float scale);
	void setLight(D3DLIGHT9 *li);
};

#endif