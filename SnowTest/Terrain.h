#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

/*
	地形类.
	通过加载灰度图来设置地形高度，TERRAIN_MAX_HEIGHT设置地形的最大高度，以此按比例缩放
	高度。


*/

const int TERRAIN_MAX_HEIGHT = 100;

struct TerrainVertex{
	float x, y, z;
	float u, v;
	static const DWORD FVF;
};

class Terrain
{
private:
	int heightmapSize;
	unsigned char *heightmapData;  //地形数据
	IDirect3DTexture9 *tex;              //纹理
	IDirect3DDevice9* dev;
	IDirect3DVertexBuffer9* vbuf;  //顶点缓冲
	IDirect3DIndexBuffer9* ibuf;   //索引缓冲

public:
	void initTexture(std::string path);
	void initHeightMap(std::string rawHeightmapPath , int size);//高度图位置与边长
	void draw();
	void generateVertex();
	
	int operator ()(int row, int col) const; //支持括号访问高度图

public:
	Terrain(IDirect3DDevice9* dev);
	~Terrain();


};


#endif