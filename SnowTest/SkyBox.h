#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

/*
	��պж����ʽ
*/
struct SkyBoxVertex
{
	float x, y, z;
	float u, v;
	static const DWORD FVF;
};

/*
��պУ�����ǰ�������ҡ��Ϲ�5���档
�����ͼƬ��Դ�ж�ȡ��
*/
class SkyBox{

private:
	IDirect3DTexture9* tex[5];       //����
	IDirect3DDevice9* dev;           //�豸
	IDirect3DVertexBuffer9* vbuf;  //���㻺��

public:
	bool createTextureFromFile(std::string filepath);

	bool initVertexs();
	SkyBox();
	virtual ~SkyBox();
};

#endif