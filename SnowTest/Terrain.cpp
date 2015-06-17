#include "Terrain.h"
#include <fstream>
#include <assert.h>

const DWORD TerrainVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

Terrain::Terrain(IDirect3DDevice9* dev) :heightmapData(0), tex(0){
	this->dev = dev;
}

Terrain::~Terrain()
{
	if (heightmapData){
		delete heightmapData;
	}
}

void Terrain::initHeightMap(std::string rawHeightmapPath , int size){
	this->heightmapSize = size;
	//读取raw文件
	std::ifstream fin(rawHeightmapPath, std::ios::binary);

	heightmapData = new unsigned char[size*size];
	
	fin.read((char *)heightmapData, size*size);
	
	fin.close();


}

void Terrain::initTexture(std::string path){
	D3DXCreateTextureFromFile(this->dev, path.c_str(), &(this->tex));
}

float Terrain::getHeight(float x, float z){
	if (x < -256 || x > 256){
		return 0;
	}
	if (z < -256 || z > 256){
		return 0;
	}
	int row = heightmapSize / 2 - z;
	int col = x + heightmapSize / 2;
	int idx = row*heightmapSize + col;

	return heightmapData[idx]/255.0 * TERRAIN_MAX_HEIGHT;
}

void Terrain::generateVertex(){
	
	HRESULT res = dev->CreateVertexBuffer(sizeof(TerrainVertex)*heightmapSize*heightmapSize, D3DUSAGE_WRITEONLY,
									TerrainVertex::FVF, D3DPOOL_MANAGED, &vbuf, 0);
	assert(SUCCEEDED(res));

	/*
	设置地形的行列数为heightmap的size。
	*/
	float delta = 1.0f / (heightmapSize - 1);

	//设置顶点数据

	TerrainVertex *vertex = NULL;

	vbuf->Lock(0, 0, (void **)&vertex, 0);


	for (int z = heightmapSize / 2; z >= -heightmapSize / 2;z -= 1){

		for (int x = -heightmapSize / 2; x <= heightmapSize / 2; x += 1){

			int row = heightmapSize / 2 - z;
			int col = x + heightmapSize / 2;
			int idx = row*heightmapSize + col;

			//顶点y值按灰度值的比例确定
			//vertex[idx] = { x, ((float)heightmapData[idx]) / 255.0 * TERRAIN_MAX_HEIGHT, z, col*delta, row * delta };
			vertex[idx] = TerrainVertex(x, ((float)heightmapData[idx]) / 255.0 * TERRAIN_MAX_HEIGHT, z, col*delta, row * delta);
			//vertex[idx] = TerrainVertex(x, 0.0f, z, col*delta, row * delta);
		}
	}

	vbuf->Unlock();


	//设置顶点索引数据
	int numTriangles = (heightmapSize - 1)*(heightmapSize - 1) * 2;
	res = dev->CreateIndexBuffer(numTriangles *3* sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &ibuf, 0);

	assert(SUCCEEDED(res));

	DWORD *index = 0;

	ibuf->Lock(0, 0, (void **)&index, 0);
	int cur = 0;

	for (int i = 0; i < heightmapSize-1; i++){
		for (int j = 0; j < heightmapSize-1; j++){

			//index[cur] = heightmapSize*i + j;
			//index[cur + 1] = heightmapSize*i + j + 1;
			//index[cur + 2] = heightmapSize*(i + 1) + j + 1;

			//index[cur + 3] = heightmapSize*(i + 1) + j + 1;
			//index[cur + 4] = heightmapSize*(i + 1) + j;
			//index[cur + 5] = heightmapSize*i + j;


			index[cur] = heightmapSize*i + j;
			index[cur + 1] = heightmapSize*i + j + 1;
			index[cur + 2] = heightmapSize*(i + 1) + j ;

			index[cur + 3] = index[cur + 2];
			index[cur + 4] = index[cur + 1];
			index[cur + 5] = heightmapSize*(i + 1) + j+1;
			cur += 6;
		}
	}


	ibuf->Unlock();

}




void Terrain::draw(){

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	dev->SetTransform(D3DTS_WORLD, &mat);

	dev->SetFVF(TerrainVertex::FVF);
	dev->SetStreamSource(0, vbuf, 0, sizeof(TerrainVertex));
	dev->SetIndices(ibuf);

	dev->SetTexture(0, this->tex);
	//dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);


	//dev->SetRenderState(D3DRS_LIGHTING, false);
	//dev->SetRenderState(D3DRS_AMBIENT, 0x00202020);

	dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//dev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, heightmapSize*heightmapSize, 0, (heightmapSize - 1)*(heightmapSize - 1) * 2);


}