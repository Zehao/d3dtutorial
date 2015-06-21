#include "SnowMan.h"


SnowMan::SnowMan(IDirect3DDevice9 *device, Terrain *terr, string path) : XFileObject(device,terr,path)
{
	
}

SnowMan::~SnowMan(){

}

/**
	����x��z�������ѩ�ˣ�y������ݵ��θ߶�ȷ��
*/
void SnowMan::draw(float x, float z){

	draw(x, z, 90);
}

void SnowMan::draw(float x, float z, float rotationAngle){
	D3DXVECTOR3 pos;
	pos.x = x;
	pos.z = z;
	pos.y = terrain->getHeight(pos.x, pos.z) + 6.0f; //��ǰλ�õĵ��θ߶ȣ�����ѩ�˸߶�


	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	//��תѩ��
	D3DXMATRIX matRotationX;
	D3DXMatrixRotationAxis(&matRotationX, &D3DXVECTOR3(1, 0, 0), D3DXToRadian(270));

	//�ı䳯��
	D3DXMATRIX matRotationY;
	D3DXMatrixRotationAxis(&matRotationY, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(rotationAngle));

	//����ѩ��
	D3DXMATRIX scaleTrans;
	D3DXMatrixScaling(&scaleTrans, 0.2f, 0.2f, 0.2f);

	D3DXMATRIX mat = scaleTrans*matRotationX*matRotationY*matTrans;

	dev->SetTransform(D3DTS_WORLD, &mat);

	for (int i = 0; i < material.size(); i++){
		material[i].Ambient.a = 1.0f;
		dev->SetMaterial(&material[i]);
		dev->SetTexture(0, texture[i]);
		mesh->DrawSubset(i);
	}
	this->renderShadow(light, x, z, &mat);
}
