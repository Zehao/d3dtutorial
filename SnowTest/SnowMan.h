#ifndef __SNOWMAN_H__
#define __SNOWMAN_H__
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include "Terrain.h"

using namespace std;

class SnowMan
{
public:
	SnowMan(IDirect3DDevice9 *device, Terrain *terr, string path);
	~SnowMan();

private:
	IDirect3DDevice9 *dev;
	Terrain *terrain;
	string xFilePath;                                  //�ļ�·��
	ID3DXMesh* mesh;                             //mesh
	vector<D3DMATERIAL9> material;      //��������
	vector<IDirect3DTexture9 *> texture;  //��������


public:
	void initMesh();
	void draw();
};


#endif