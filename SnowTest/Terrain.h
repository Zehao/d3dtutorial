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

struct TerrainVertex{
	float x, y, z;
	float u, v;
	static const DWORD FVF;
	TerrainVertex(float _x, float _y, float _z, float _u, float _v){
		x = _x, y = _y, z = _z, u = _u, v = _v;
	}
};

class Terrain
{
private:
	int heightmapSize;
	int texScale;                                     //��Ӧ��ͼ�ķŴ����
	unsigned char *heightmapData;  //��������
	IDirect3DTexture9 *tex;              //����
	IDirect3DDevice9* dev;
	IDirect3DVertexBuffer9* vbuf;  //���㻺��
	IDirect3DIndexBuffer9* ibuf;   //��������

public:
	void initTexture(std::string path);
	void initHeightMap(std::string rawHeightmapPath , int size,int scale);//�߶�ͼλ����߳�
	void draw();
	void generateVertex();

	float getHeight(float x, float z);
	
public:
	Terrain(IDirect3DDevice9* dev);
	~Terrain();


};


#endif