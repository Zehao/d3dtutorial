#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

/*
	������.
	ͨ�����ػҶ�ͼ�����õ��θ߶ȣ�TERRAIN_MAX_HEIGHT���õ��ε����߶ȣ��Դ˰���������
	�߶ȡ�


*/

const int TERRAIN_MAX_HEIGHT = 100;

class Terrain
{
private:
	int heightmapSize;
	unsigned char *heightmapData;  //��������
	IDirect3DTexture9 *tex;              //����
	IDirect3DDevice9* dev;


public:
	void initTexture(std::string path);
	void initHeightMap(std::string rawHeightmapPath , int size);//�߶�ͼλ����߳�
	void draw();

public:
	Terrain(IDirect3DDevice9* dev);
	~Terrain();


};


#endif