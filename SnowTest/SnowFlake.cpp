#include "SnowFlake.h"

const DWORD SnowFlakeVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;


SnowFlakeManager::SnowFlakeManager(IDirect3DDevice9 *device, Terrain *terr , string snowTexPath, int flakeCnt){

	dev = device;

	terrain = terr;

	D3DXCreateTextureFromFile(dev, snowTexPath.c_str(), &tex);

	vbuf = NULL;
	snowFlakeNum = flakeCnt;

	snowFlake = new SnowFlake[snowFlakeNum];
	snowFlakeSize = 2;
	
}

void SnowFlakeManager::initSnowFlakes(){
	

	HRESULT result = dev->CreateVertexBuffer(snowFlakeNum * sizeof(SnowFlake),
								0,SnowFlakeVertex::FVF, //动态缓冲
								D3DPOOL::D3DPOOL_MANAGED, &vbuf, 0);
	if (FAILED(result)){
		MessageBox(0, "SnowFlakeManager::initSnowFlakes CreateVertexBuffer failed",0, 0);
	}

	SnowFlakeVertex *vertex;
	vbuf->Lock(0, snowFlakeNum, (void **)&vertex, 0);
	srand((int)timeGetTime());
	//设置属性值
	for (int i = 0; i < snowFlakeNum; i++){
		vertex[i].x = rand() % SNOW_FLAKE_MAX_X;
		vertex[i].y = SNOW_FLAKE_MAX_Z;
		vertex[i].z = rand() % SNOW_FLAKE_MAX_X;
		snowFlake[i].velocity = rand() % 100 + 100.0f;
	}
	vbuf->Unlock();
}

void SnowFlakeManager::updateSnowFlakes(float deltaTime){

	SnowFlakeVertex *vertex;
	vbuf->Lock(0, snowFlakeNum, (void **)&vertex, 0);

	for (int i = 0; i < snowFlakeNum; i++){
		vertex[i].y -= snowFlake[i].velocity*deltaTime;
		if (vertex[i].y < terrain->getHeight(vertex[i].x, vertex[i].z)){
			vertex[i].y = SNOW_FLAKE_MAX_Z;
		}

		//snowFlake[i].position.y -= snowFlake[i].velocity*deltaTime;
		//snowFlake[i].rotationY += snowFlake[i].rotationSpeed;
		////雪花到达地表
		//if (snowFlake[i].position.y < terrain->getHeight(snowFlake[i].position.x, snowFlake[i].position.z)){
		//	snowFlake[i].position.y = SNOW_FLAKE_MAX_Z;
		//}
	}
	vbuf->Unlock();

}

void SnowFlakeManager::draw(){

	dev->SetRenderState(D3DRS_LIGHTING, false);
	dev->SetRenderState(D3DRS_POINTSPRITEENABLE, true); //映射整个纹理
	dev->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	dev->SetRenderState(D3DRS_POINTSIZE, snowFlakeSize);
	dev->SetRenderState(D3DRS_POINTSIZE_MIN, 1);

	//控制粒子远近大小
	dev->SetRenderState(D3DRS_POINTSCALE_A,0);
	dev->SetRenderState(D3DRS_POINTSCALE_B, 0);
	dev->SetRenderState(D3DRS_POINTSCALE_C, 1);

	//从纹理中获取alpha通道
	dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//将纹理颜色值输出  
	dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);  
	dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 

	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	dev->SetTexture(0, tex);
	dev->SetFVF(SnowFlakeVertex::FVF);
	dev->SetStreamSource(0, vbuf, 0, sizeof(SnowFlakeVertex));
	dev->DrawPrimitive(D3DPT_POINTLIST, 0, snowFlakeNum);

	//恢复设置 
	dev->SetRenderState(D3DRS_LIGHTING, true);
	dev->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	dev->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

SnowFlakeManager::~SnowFlakeManager(){
	if (snowFlake){
		delete snowFlake;
	}
	vbuf->Release();
	tex->Release();
}