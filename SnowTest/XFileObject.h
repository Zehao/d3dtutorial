#ifndef __XFILEOBJECT_H__
#define __XFILEOBJECT_H__
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include "Terrain.h"
using namespace std;

/**
	X �ļ��࣬�ɾ��������ʵ��draw��
*/
class XFileObject{

protected:
	IDirect3DDevice9 *dev;
	Terrain *terrain;
	string xFilePath;                                  //�ļ�·��
	ID3DXMesh* mesh;                             //mesh
	vector<D3DMATERIAL9> material;      //��������
	vector<IDirect3DTexture9 *> texture;  //��������


public:
	XFileObject(IDirect3DDevice9 *device, Terrain *terr, string path); 

	virtual ~XFileObject();

public :
	void initMesh();
	virtual void draw(float x, float z ) = 0;
};

#endif
