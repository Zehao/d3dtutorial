#include "SnowMan.h"


SnowMan::SnowMan(IDirect3DDevice9 *device, string path) :dev(device), xFilePath(path)
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