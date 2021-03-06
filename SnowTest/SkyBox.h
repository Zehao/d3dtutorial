#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "Camera.h"

/*
	天空盒边长，其为一个立方体
*/
static const int SKYBOX_SIZE = 1024;

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

天空盒顶点包括位置坐标和纹理坐标，顶点数据通过D3DPT_TRIANGLESTRIP模式
绘制时，可以不需要索引缓冲。

纹理从图片资源中读取。

*/
class SkyBox {

private:
	IDirect3DTexture9* tex[5];       //纹理
	IDirect3DDevice9* dev;           //设备
	IDirect3DVertexBuffer9* vbuf;  //顶点缓冲


public:
	void initTexture(std::string filepath);

	void initVertexs();

	void draw(Camera* cam);

public:

	SkyBox(IDirect3DDevice9* dev);

	~SkyBox();
};

#endif