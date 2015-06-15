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
	
	fin.read((char *)heightmapData, size*size*sizeof(char));

	fin.close();


}

void Terrain::initTexture(std::string path){
	D3DXCreateTextureFromFile(this->dev, path.c_str(), &(this->tex));
}

int Terrain::operator()(int row, int col) const{
	return heightmapData[row*heightmapSize + col];
}

void Terrain::generateVertex(){
	
	HRESULT res = dev->CreateVertexBuffer(sizeof(TerrainVertex)*heightmapSize*heightmapSize, D3DUSAGE_WRITEONLY,
									TerrainVertex::FVF, D3DPOOL_MANAGED, &vbuf, 0);
	assert(SUCCEEDED(res));

	/*
	设置地形的行列数为heightmap的size。
	*/
	int numRow = heightmapSize - 1;
	int numCol = heightmapSize - 1;
	float delta = 1.0f / numRow;

	//设置顶点数据

	TerrainVertex *vertex = NULL;

	vbuf->Lock(0, heightmapSize*heightmapSize, (void **)&vertex, 0);

	int row = 0;
	for (int x = -heightmapSize / 2; x <= heightmapSize / 2; x += 2){
		int col = 0;
		for (int z = heightmapSize / 2; z >= -heightmapSize / 2; z -= 2){
			int idx = row*heightmapSize + col;
			vertex[idx] = { x, ((float)heightmapData[idx]) / 255 * TERRAIN_MAX_HEIGHT, z, col*delta, row * delta };
			col++;
		}
		row++;
	}

	vbuf->Unlock();


	//设置顶点索引数据
	int numTriangles = (heightmapSize - 1)*(heightmapSize - 1) * 2;
	res = dev->CreateIndexBuffer(numTriangles * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ibuf, 0);

	assert(SUCCEEDED(res));

	WORD *index = 0;

	for (int i = 0; i < numRow; i++){
		for (int j = 0; j < numCol; j++){

		}
	}

}

void Terrain::draw(){



}