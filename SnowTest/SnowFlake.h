#ifndef __SNOWFLAKE_H__
#define __SNOWFLAKE_H__
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "Terrain.h"

using namespace std;

//ѩ������
struct SnowFlakeVertex
{
	float x, y, z;
	float u, v;
	static const DWORD FVF;
};

/*
	ѩ��
*/
struct SnowFlake
{
	D3DXVECTOR3 position;
	float velocity;
	float rotationY;
	float rotationSpeed; //��Y����ת�ٶ�

};

//��Χ
const int SNOW_FLAKE_MAX_X = 512;
const int SNOW_FLAKE_MAX_Z = 800;

/*
	ѩ��������
*/
class SnowFlakeManager{

private:

	IDirect3DDevice9* dev;
	IDirect3DTexture9* tex;
	IDirect3DVertexBuffer9 *vbuf;
	Terrain *terrain;

private:
	SnowFlake *snowFlake; //ѩ����������
	int snowFlakeNum;       //����
	int snowFlakeSize;



public:
	SnowFlakeManager(IDirect3DDevice9 *device, Terrain *terrain , string snowTexPath, int flakeCnt);
	void initSnowFlakes();
	void updateSnowFlakes(float deltaTime);

	void draw();

	~SnowFlakeManager();
};

#endif 