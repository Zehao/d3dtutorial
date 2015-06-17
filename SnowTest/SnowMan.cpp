#include "SnowMan.h"


SnowMan::SnowMan(IDirect3DDevice9 *device, Terrain *terr, string path) :dev(device),terrain(terr), xFilePath(path)
{
}

SnowMan::~SnowMan()
{
	if (mesh){
		mesh->Release();
	}
	for (int i = 0; i < texture.size(); i++)
		texture[i]->Release();
}


void SnowMan::initMesh(){

	ID3DXBuffer* adjBuf = 0;
	ID3DXBuffer* mtrlBuf = 0;
	DWORD mtrlCnt;

	HRESULT result = D3DXLoadMeshFromX(xFilePath.c_str(), D3DXMESH_MANAGED, dev, 
									&adjBuf, &mtrlBuf,0, &mtrlCnt, &mesh);
	if (FAILED(result)){
		MessageBox(0, "load xfile failed", 0, 0);
	}

	if (mtrlBuf != 0 && mtrlCnt != 0){
		D3DXMATERIAL *mtrls = (D3DXMATERIAL *)mtrlBuf->GetBufferPointer();
		for (int i = 0; i < mtrlCnt; i++){
			//默认无环境光，添加材质的环境光
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			material.push_back(mtrls[i].MatD3D);
			//材质附带纹理
			if (mtrls[i].pTextureFilename){
				IDirect3DTexture9 * tex;
				D3DXCreateTextureFromFile(dev, mtrls[i].pTextureFilename, &tex);
				texture.push_back(tex);
			}
			else{
				texture.push_back(0);
			}

		}
	}
	mtrlBuf->Release();

}

void SnowMan::draw(){

	D3DXVECTOR3 pos;
	pos.x = 0; 
	pos.z = 0;
	pos.y = terrain->getHeight(pos.x, pos.z)+2.0f;


	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	D3DXMATRIX matRotation;
	D3DXMatrixRotationAxis(&matRotation, &D3DXVECTOR3(1, 0, 0), D3DXToRadian(270)); //针对任意轴旋转
	
	D3DXMATRIX mat = matRotation*matTrans;
	dev->SetTransform(D3DTS_WORLD, &mat);



	//需要打上灯光
	D3DLIGHT9 light;
	memset(&light, 0, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DXVECTOR3(-1,-1,1);//根据天空盒太阳位置得出
	D3DXCOLOR white = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Ambient = white;
	//light.Diffuse = white*0.4f;
	//light.Specular = white * 0.6f;

	dev->SetRenderState(D3DRS_ZENABLE, TRUE);
	dev->SetRenderState(D3DRS_LIGHTING, TRUE); //灯光
	dev->SetLight(0, &light);
	dev->LightEnable(0, true);

	for (int i = 0; i < material.size(); i++){
		dev->SetMaterial(&material[i]);
		dev->SetTexture(0, texture[i]);
		mesh->DrawSubset(i);
	}
	dev->SetRenderState(D3DRS_LIGHTING, FALSE); //灯光
}