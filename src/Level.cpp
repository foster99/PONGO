#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Level.h"


using namespace std;



Level::Level(int id)
	: levelID(id)
{
	this->World::World();

	loadTileMap();
}

Level::~Level()
{
	this->World::~World();
}

void Level::free()
{
	this->World::free();
}

void Level::render() const
{
	this->World::render();

	// render TileMap
	// render Ball
	// render Slides
}


// TileMap

bool diff(const std::string& line1, const std::string& line2)
{
	int length = line2.length();
	return line1.compare(0, length, line2) != 0;
}

void Level::loadTileMap()
{
	// CALCULAR EL PATH
	string levelFilePath;
	if (levelID < 10)	levelFilePath = "levels/LEVEL_0" + to_string(levelID) + ".txt";
	else				levelFilePath = "levels/LEVEL_"  + to_string(levelID) + ".txt";



	// INICIALIZAR EL LECTOR
	ifstream fin;
	string line;
	stringstream sstream;
	int i, j, i0, i1, j0, j1;
	fin.open(levelFilePath.c_str());
	if (!fin.is_open())	throw "Could not load the file.";



	// START LOADING LEVEL
	getline(fin, line);
	if (diff(line, "LEVEL")) throw "LEVEL NOT FOUND";
	getline(fin, line);	// Blank line



	// LOAD HORIZONTAL SLIDES
	getline(fin, line);	// Blank line
	getline(fin, line);
	if (diff(line, "START  - HORIZONTAL SLIDES"))
		throw std::string("Error parsing " + line).c_str();
	
	getline(fin, line);
	while (diff(line, "END    - HORIZONTAL SLIDES"))
	{
		sstream.str(line);
		sstream >> i >> j0 >> j1;
		//slides.push_back(Slide(Slide::Horizontal, i, j0, j1));
		getline(fin, line);
	}



	// LOAD VERTICAL SLIDES
	getline(fin, line); // Blank line
	getline(fin, line);
	if (diff(line, "START  - VERTICAL SLIDES"))
		throw std::string("Error parsing " + line).c_str();

	getline(fin, line);
	while (diff(line, "END    - VERTICAL SLIDES"))
	{
		sstream.str(line);
		sstream >> j >> i0 >> i1;
		//slides.push_back(Slide(Slide::Vertical, j, i0, i1));
		getline(fin, line);
	}
	

	// LOAD LEVEL PARAMETERS
	getline(fin, line);	// Blank line
	getline(fin, line);
	if (diff(line, "START  - TILEMAP"))
		throw std::string("Error parsing " + line).c_str();
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize;

	getline(fin, line);
	sstream.str(line);
	sstream >> chunkSize.y >> chunkSize.x;

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSizeInChunks.y >> mapSizeInChunks.x;

	mapSizeInTiles = mapSizeInChunks * chunkSize;

	// INICIALIZAR Y LEER TILE MAP
	char tile;
	map = vector<vector<Tile>>(mapSizeInTiles.y, vector<Tile>(mapSizeInTiles.x));
	
	getline(fin, line); // Blank line
	for (int i = 0; i < mapSizeInTiles.y; i++)
	{
		for (int j = 0; j < mapSizeInTiles.x; j++)
		{
			fin.get(tile);
			loadTile(tile, i, j);
		}

		fin.get(tile);

		#ifndef _WIN32
			fin.get(tile);
		#endif
	}

	getline(fin, line); // Blank line
	getline(fin, line);
	if (diff(line, "END    - TILEMAP"))
		throw std::string("Error parsing " + line).c_str();

	fin.close();
}

void Level::loadTile(char tile, int i, int j)
{
	// falta codigo
}





























