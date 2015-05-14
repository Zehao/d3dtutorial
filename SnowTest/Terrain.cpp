#include "Terrain.h"
#include <fstream>

Terrain::Terrain():heightmapData(0){

}

Terrain::~Terrain()
{
}

void Terrain::initHeightMap(std::string rawHeightmapPath , int size){
	this->heightmapSize = size;
	std::ifstream fin(rawHeightmapPath, std::ios::binary);

	heightmapData = new unsigned char[size*size];
	
	fin.read((char *)heightmapData, size*size*sizeof(char));

	fin.close();


}