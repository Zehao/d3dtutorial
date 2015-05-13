#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

/*
	天空盒顶点格式
*/
struct SkyBoxVertex
{
	float x, y, z;
	float u, v;
	static const DWORD FVF;
};

/*
天空盒，包含前、后、左、右、上共5个面。
纹理从图片资源中读取。
*/
class SkyBox{

private:
	IDirect3DTexture9* tex[5];       //纹理
	IDirect3DDevice9* dev;           //设备
	IDirect3DVertexBuffer9* vbuf;  //顶点缓冲

public:
	bool createTextureFromFile(std::string filepath);

	bool initVertexs();
	SkyBox();
	virtual ~SkyBox();
};

#endif