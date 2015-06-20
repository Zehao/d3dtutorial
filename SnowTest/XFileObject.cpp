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

			////mtrls[i].MatD3D.Ambient.a = 1;
			//mtrls[i].MatD3D.Diffuse.a = 1.0f;
			//mtrls[i].MatD3D.Emissive.a = 1.0f;
			//mtrls[i].MatD3D.Specular.a = 1.0f;
			//mtrls[i].MatD3D.Ambient.a = 1.0f;
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

void XFileObject::renderShadow(D3DLIGHT9 *light,float x ,float z, D3DXMATRIX *transMat){

	dev->SetRenderState(D3DRS_STENCILENABLE, true);
	dev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	dev->SetRenderState(D3DRS_STENCILREF, 0x0);
	dev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	dev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	dev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	dev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	dev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR); // increment to 1


	float height = terrain->getHeight(x, z);

	//平面,根据当前地形高度确定顶点
	D3DXPLANE plane;
	D3DXPlaneFromPointNormal(&plane, &D3DXVECTOR3(x, height+0.1f, z), &D3DXVECTOR3(0, -1, 0));

	//光照
	D3DXVECTOR3 lightDir(light->Direction);
	D3DXVec3Normalize(&lightDir, &lightDir);

	D3DXVECTOR4 lightDirection(lightDir.x, lightDir.y, lightDir.z, 0.0f);

	D3DXMATRIX S;
	D3DXMatrixShadow(
		&S,
		&lightDirection,
		&plane);

	D3DXMATRIX W = (*transMat) * S;
	dev->SetTransform(D3DTS_WORLD, &W);

	// alpha blend the shadow
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	

	D3DMATERIAL9 mtrl;
	mtrl.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	mtrl.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	mtrl.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	mtrl.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	mtrl.Power = 0;
	mtrl.Diffuse.a = 0.4f; // 50% transparency.

	// Disable depth buffer so that z-fighting doesn't occur when we
	// render the shadow on top of the floor.
	dev->SetRenderState(D3DRS_ZENABLE, true);

	dev->SetMaterial(&mtrl);
	dev->SetTexture(0, 0);
	for (int i = 0; i < material.size(); i++){
		mesh->DrawSubset(i);
	}

	dev->SetRenderState(D3DRS_ZENABLE, true);
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	dev->SetRenderState(D3DRS_STENCILENABLE, false);
}
