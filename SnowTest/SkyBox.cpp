#include "SkyBox.h"

const DWORD SkyBoxVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

bool SkyBox::initVertexs(){
	this->dev->CreateVertexBuffer(20 * sizeof(SkyBoxVertex), 0, SkyBoxVertex::FVF, D3DPOOL::D3DPOOL_MANAGED, &vbuf, NULL);

	int ss = 256; //half sky size
	SkyBoxVertex vertex[] = {
		
		//back，这里要考虑背面选剪时的方向，顺序与front相反
		{ ss, 0, -ss, 0, 1 },
		{ ss, ss, -ss, 0, 0 },
		{ -ss, ss, -ss, 1, 1 },
		{ -ss, 0, -ss, 1, 0 },

		//front
		{ -ss, 0, ss, 0, 1 },
		{ -ss, ss, ss, 0, 0 },
		{ ss, 0, ss, 1, 1 },
		{ ss, ss, ss, 1, 0 },

		//left
		{ -ss, 0, -ss, 0, 1 },
		{ -ss, ss, -ss, 0, 0 },
		{ ss, 0, ss, 1, 1 },
		{ ss, ss, ss, 1, 0 },

		//right
		{ ss, 0, ss, 0, 1 },
		{ ss, ss, ss, 0, 0 },
		{ ss, 0, -ss, 1, 1 },
		{ss, ss, -ss, 1, 0},
		
		//top
		{ -ss, ss, ss, 0, 1},
		{ -ss, ss, -ss, 0, 0 },
		{ ss, ss, -ss, 1, 1 },
		{ ss, ss, ss, 1, 0 }
	};

}