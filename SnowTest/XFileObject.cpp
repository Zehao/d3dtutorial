#include "XFileObject.h"

XFileObject::XFileObject(IDirect3DDevice9 *device, Terrain *terr, string path) :dev(device), terrain(terr), xFilePath(path){

}

XFileObject::~XFileObject(){
	if (mesh){
		mesh->Release();
	}
	for (int i = 0; i < texture.size(); i++)
		texture[i]->Release();
}

void XFileObject::initMesh(){

	ID3DXBuffer* adjBuf = 0;
	ID3DXBuffer* mtrlBuf = 0;
	DWORD mtrlCnt;

	HRESULT result = D3DXLoadMeshFromX(xFilePath.c_str(), D3DXMESH_MANAGED, dev,
		&adjBuf, &mtrlBuf, 0, &mtrlCnt, &mesh);
	if (FAILED(result)){
		MessageBox(0, "load xfile failed", 0, 0);
		return;
	}

	if (mtrlBuf != 0 && mtrlCnt != 0){
		D3DXMATERIAL *mtrls = (D3DXMATERIAL *)mtrlBuf->GetBufferPointer();
		for (int i = 0; i < mtrlCnt; i++){
			//默认无环境光，添加材质的环境光

			//mtrls[i].MatD3D.Ambient.a = 1;
			mtrls[i].MatD3D.Diffuse.a = 1.0f;
			mtrls[i].MatD3D.Emissive.a = 1.0f;
			mtrls[i].MatD3D.Specular.a = 1.0f;
			mtrls[i].MatD3D.Ambient.a = 1.0f;
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			material.push_back(mtrls[i].MatD3D);

			//材质附带纹理
			if (mtrls[i].pTextureFilename){
				IDirect3DTexture9 * tex;
				int idx = (int)xFilePath.find_last_of('/');
				string s = xFilePath.substr(0, idx) + "/" + mtrls[i].pTextureFilename;
				D3DXCreateTextureFromFile(dev, s.c_str(), &tex);
				texture.push_back(tex);
			}
			else{
				texture.push_back(0);
			}

		}
	}
	mtrlBuf->Release();

	mesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuf->GetBufferPointer(),
		0, 0, 0);
	adjBuf->Release();
}
