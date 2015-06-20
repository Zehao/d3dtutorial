#include "Tree.h"

Tree::Tree(IDirect3DDevice9 *device, Terrain *terr, string path) :XFileObject(device, terr, path){

}

void Tree::draw(float x, float z){
	draw(x, z, 8.0f);
}


void Tree::draw(float x, float z ,float scale){
	D3DXVECTOR3 pos;
	pos.x = x;
	pos.z = z;
	pos.y = terrain->getHeight(pos.x, pos.z) + 0.5f; //当前位置的地形高度，调整高度


	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	D3DXMATRIX matRotation;
	D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(1, 0, 0), D3DXToRadian(270));

	//缩放雪人
	D3DXMATRIX scaleTrans;
	D3DXMatrixScaling(&scaleTrans, scale, scale, scale);

	D3DXMATRIX mat = scaleTrans*matRotation*matTrans;

	dev->SetTransform(D3DTS_WORLD, &mat);

	for (int i = 0; i < material.size(); i++){
		material[i].Ambient.a = 1.0f;
		dev->SetMaterial(&material[i]);
		dev->SetTexture(0, texture[i]);
		mesh->DrawSubset(i);
	}

	this->renderShadow(light, x, z, &mat);
}

void Tree::setLight(D3DLIGHT9 *li){
	light = li;
}