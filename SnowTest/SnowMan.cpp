#include "SnowMan.h"


SnowMan::SnowMan(IDirect3DDevice9 *device, Terrain *terr, string path) : XFileObject(device,terr,path)
{
	
}

SnowMan::~SnowMan(){

}

/**
	根据x和z坐标放置雪人，y坐标根据地形高度确定
*/
void SnowMan::draw(float x, float z){

	draw(x, z, 90);
}

void SnowMan::draw(float x, float z, float rotationAngle){
	D3DXVECTOR3 pos;
	pos.x = x;
	pos.z = z;
	pos.y = terrain->getHeight(pos.x, pos.z) + 6.0f; //当前位置的地形高度，调整雪人高度


	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	//旋转雪人
	D3DXMATRIX matRotationX;
	D3DXMatrixRotationAxis(&matRotationX, &D3DXVECTOR3(1, 0, 0), D3DXToRadian(270));

	//改变朝向
	D3DXMATRIX matRotationY;
	D3DXMatrixRotationAxis(&matRotationY, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(rotationAngle));

	//缩放雪人
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
