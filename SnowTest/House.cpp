#include "House.h"


House::House(IDirect3DDevice9 *device, Terrain *terr, string path) : XFileObject(device, terr, path)
{

}

House::~House(){

}


void House::draw(float x, float z){

	D3DXVECTOR3 pos;
	pos.x = x;
	pos.z = z;
	pos.y = terrain->getHeight(pos.x, pos.z) + 26.0f; //调整房子高度

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	dev->SetTransform(D3DTS_WORLD, &matTrans);

	for (int i = 0; i < material.size(); i++){
		material[i].Ambient.a = 1.0f;
		dev->SetMaterial(&material[i]);
		dev->SetTexture(0, texture[i]);
		mesh->DrawSubset(i);
	}
}