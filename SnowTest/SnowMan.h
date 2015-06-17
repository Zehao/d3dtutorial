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
	string xFilePath;                                  //文件路径
	ID3DXMesh* mesh;                             //mesh
	vector<D3DMATERIAL9> material;      //材质数组
	vector<IDirect3DTexture9 *> texture;  //纹理数组


public:
	void initMesh();
};


#endif