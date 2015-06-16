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
	���ļ���ȡ����,����ǰ�������ҡ��ϵ�˳��
*/

void SkyBox::initTexture(std::string filepath){

	std::string path[] = { "front", "back", "left", "right", "top" };

	for (int i = 0; i < 5; i++){
		std::string tmp = filepath + "//" + path[i] +".jpg";
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
		
		//back������Ҫ���Ǳ���ѡ��ʱ�ķ���˳����front�෴
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

void SkyBox::draw(Camera* cam){



	dev->SetFVF(SkyBoxVertex::FVF);
	//dev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);//Mipmaps������

	//����Ѱַ��ʽΪ����ģʽ
	dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	//��պи���������ƶ�
	D3DXVECTOR3 pos;
	cam->getPosition(&pos);
	D3DXMATRIX matPre,mat;

	dev->GetTransform(D3DTS_WORLD, &matPre);
	D3DXMatrixTranslation(&mat, pos.x, 0, pos.z); //y����
	dev->SetTransform(D3DTS_WORLD, &mat);

	//dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	dev->SetStreamSource(0, vbuf, 0, sizeof(SkyBoxVertex));

	for (int i = 0; i < 5; i++){
		dev->SetTexture(0, tex[i]);
		dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);//ÿ4�������2��������
	}

	dev->SetTransform(D3DTS_WORLD, &matPre); //��ԭ�������
}