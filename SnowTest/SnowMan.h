#ifndef __SNOWMAN_H__
#define __SNOWMAN_H__
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>

using namespace std;

class SnowMan
{
public:
	SnowMan(IDirect3DDevice9 *device, string path);
	~SnowMan();

private:
	IDirect3DDevice9 *dev;
	string xFilePath;                                  //�ļ�·��
	ID3DXMesh* mesh;                             //mesh
	vector<D3DMATERIAL9> material;      //��������
	vector<IDirect3DTexture9 *> texture;  //��������


public:
	void initMesh();
};


#endif