#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class Terrain
{
private:
	int heightmapSize;
	unsigned char *heightmapData;

public:
	void initTexture(std::string path);
	void initHeightMap(std::string rawHeightmapPath , int size);//�߶�ͼλ����߳�
	void draw();

public:
	Terrain();
	~Terrain();


};


#endif