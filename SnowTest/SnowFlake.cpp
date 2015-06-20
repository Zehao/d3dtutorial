#include "SnowFlake.h"

const DWORD SnowFlakeVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;


SnowFlakeManager::SnowFlakeManager(IDirect3DDevice9 *device, Terrain *terr , string snowTexPath, int flakeCnt){

	dev = device;

	terrain = terr;

	HRESULT res = D3DXCreateTextureFromFile(dev, snowTexPath.c_str(), &tex);
	
	if (FAILED(res)){
		MessageBox(0, "SnowFlakeManager create texture failed.", 0, 0);
		return;
	}

	vbuf = NULL;
	snowFlakeNum = flakeCnt;

	snowFlake = new SnowFlake[snowFlakeNum];
}

void SnowFlakeManager::initSnowFlakes(){
	

	HRESULT result = dev->CreateVertexBuffer(sizeof(SnowFlake)*4,
								0,SnowFlakeVertex::FVF, //动态缓冲
								D3DPOOL::D3DPOOL_MANAGED, &vbuf, 0);
	if (FAILED(result)){
		MessageBox(0, "SnowFlakeManager::initSnowFlakes CreateVertexBuffer failed",0, 0);
	}

	SnowFlakeVertex *vertex;

	float s = 1.5;
	SnowFlakeVertex snowPlane[] = {
		{-s, -s ,0 , 0, 1},
		{ -s, s, 0, 0, 0 },
		{ s, -s, 0, 1, 1 },
		{ s, s, 0, 1, 0 }
	};

	vbuf->Lock(0, 0, (void **)&vertex, 0);

	memcpy(vertex, snowPlane, sizeof(snowPlane));

	vbuf->Unlock();
	srand((int)timeGetTime());
	//设置属性值
	for (int i = 0; i < snowFlakeNum; i++){
		snowFlake[i].position.x = rand() % SNOW_FLAKE_MAX_X - SNOW_FLAKE_MAX_X/2;
		snowFlake[i].position.y = SNOW_FLAKE_MAX_Y;
		snowFlake[i].position.z = rand() % SNOW_FLAKE_MAX_X - SNOW_FLAKE_MAX_X / 2;
		snowFlake[i].rotationX = rand() % 5;
		snowFlake[i].rotationY = rand() % 5;
		snowFlake[i].rotationSpeed = rand() % 5/5.0f;
		snowFlake[i].velocity = rand() % 50 + 30.0f;
	}
	
}

void SnowFlakeManager::updateSnowFlakes(float deltaTime){

	for (int i = 0; i < snowFlakeNum; i++){
		snowFlake[i].position.y -= snowFlake[i].velocity*deltaTime;
		snowFlake[i].rotationX += snowFlake[i].rotationSpeed;
		snowFlake[i].rotationY += snowFlake[i].rotationSpeed + 1;
		//雪花到达地表
		if (snowFlake[i].position.y < terrain->getHeight(snowFlake[i].position.x, snowFlake[i].position.z)){
			snowFlake[i].position.y = SNOW_FLAKE_MAX_Y;
		}
	}
}

void SnowFlakeManager::draw(){

	
	//从纹理中获取alpha通道
	dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//将纹理颜色值输出  
	//dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);  
	//dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	//dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );
	//dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//源混合是当前纹理alpha，背景的alpha为1
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//不采用背面拣选
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	DWORD lightValue;
	dev->GetRenderState(D3DRS_LIGHTING, &lightValue);

	dev->SetRenderState(D3DRS_LIGHTING, FALSE);

	dev->SetTexture(0, tex);
	dev->SetFVF(SnowFlakeVertex::FVF);

	for (int i = 0; i < snowFlakeNum; i++){
		static D3DXMATRIX trans, rotationX,rotationY,rotationZ, world;
		D3DXMatrixTranslation(&trans, snowFlake[i].position.x, snowFlake[i].position.y, snowFlake[i].position.z);
		D3DXMatrixRotationX(&rotationX, D3DXToRadian(snowFlake[i].rotationX));
		D3DXMatrixRotationY(&rotationY, D3DXToRadian(snowFlake[i].rotationY));
		world = rotationX*rotationY*trans;
		dev->SetTransform(D3DTS_WORLD, &world);
		dev->SetStreamSource(0, vbuf, 0,sizeof(SnowFlakeVertex));
		dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}
	//恢复设置 
	dev->SetRenderState(D3DRS_LIGHTING, lightValue);
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

SnowFlakeManager::~SnowFlakeManager(){
	if (snowFlake){
		delete snowFlake;
	}
	vbuf->Release();
	tex->Release();
}