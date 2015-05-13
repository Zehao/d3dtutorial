#include "SkyBox.h"
#include <vector>

const DWORD SkyBoxVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;


SkyBox::SkyBox(IDirect3DDevice9* dev) :dev(dev), vbuf(0){
}

SkyBox::~SkyBox(){
	if (vbuf){
		//delete vbuf;
		vbuf->Release();
	}

	for (int i = 0; i < 5; i++){
		//if (this->tex[i]){
		//	delete this->tex[i];
		//}
		this->tex[i]->Release();
	}
}


/*
	从文件读取纹理,按照前、后、左、右、上的顺序。
*/

void SkyBox::initTexture(std::string filepath){

	std::string path[] = { "front", "back", "left", "right", "top" };

	for (int i = 0; i < 5; i++){
		std::string tmp = filepath + "//" + path[i];
		D3DXCreateTextureFromFile(this->dev, tmp.c_str(), &(this->tex[i]));
	}

}

void SkyBox::initVertexs(){
	this->dev->CreateVertexBuffer(20 * sizeof(SkyBoxVertex), 0, SkyBoxVertex::FVF, D3DPOOL::D3DPOOL_MANAGED, &vbuf, NULL);

	int ss = SKYBOX_SIZE/2; //half sky size
	SkyBoxVertex vertex[] = {

		//front
		{ -ss, 0, ss, 0, 1 },
		{ -ss, ss, ss, 0, 0 },
		{ ss, 0, ss, 1, 1 },
		{ ss, ss, ss, 1, 0 },
		
		//back，这里要考虑背面选剪时的方向，顺序与front相反
		{ ss, 0, -ss, 0, 1 },
		{ ss, ss, -ss, 0, 0 },
		{ -ss, 0, -ss, 1, 1 },
		{ -ss, ss, -ss, 1, 0 },

		//left
		{ -ss, 0, -ss, 0, 1 },
		{ -ss, ss, -ss, 0, 0 },
		{ -ss, 0, ss, 1, 1 },
		{ -ss, ss, ss, 1, 0 },

		//right
		{ ss, 0, ss, 0, 1 },
		{ ss, ss, ss, 0, 0 },
		{ ss, 0, -ss, 1, 1 },
		{ ss, ss, -ss, 1, 0},
		
		//top
		{ -ss, ss, ss, 0, 1},
		{ -ss, ss, -ss, 0, 0 },
		{ ss, ss, ss, 1, 1 },
		{ ss, ss, -ss, 1, 0 }
	};

	VOID *tmp;
	vbuf->Lock(0, 0, &tmp, 0);

	memcpy(tmp, vertex, sizeof(vertex));

	vbuf->Unlock();
}
