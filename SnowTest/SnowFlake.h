#ifndef __SNOWFLAKE_H__
#define __SNOWFLAKE_H__
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "Terrain.h"

using namespace std;

//雪花顶点
struct SnowFlakeVertex
{
	float x, y, z;
	float u, v;
	static const DWORD FVF;
};

/*
	雪花
*/
struct SnowFlake
{
	D3DXVECTOR3 position;
	float velocity;
	float rotationY;
	float rotationSpeed; //绕Y轴旋转速度

};

//范围
const int SNOW_FLAKE_MAX_X = 512;
const int SNOW_FLAKE_MAX_Z = 800;

/*
	雪花管理类
*/
class SnowFlakeManager{

private:

	IDirect3DDevice9* dev;
	IDirect3DTexture9* tex;
	IDirect3DVertexBuffer9 *vbuf;
	Terrain *terrain;

private:
	SnowFlake *snowFlake; //雪花粒子数组
	int snowFlakeNum;       //数量
	int snowFlakeSize;



public:
	SnowFlakeManager(IDirect3DDevice9 *device, Terrain *terrain , string snowTexPath, int flakeCnt);
	void initSnowFlakes();
	void updateSnowFlakes(float deltaTime);

	void draw();

	~SnowFlakeManager();
};

#endif 