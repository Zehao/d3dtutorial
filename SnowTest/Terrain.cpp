#include "Terrain.h"
#include <fstream>

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
	//��ȡraw�ļ�
	std::ifstream fin(rawHeightmapPath, std::ios::binary);

	heightmapData = new unsigned char[size*size];
	
	fin.read((char *)heightmapData, size*size*sizeof(char));

	fin.close();


}

void Terrain::initTexture(std::string path){
	D3DXCreateTextureFromFile(this->dev, path.c_str(), &(this->tex));
}

void Terrain::draw(){

}