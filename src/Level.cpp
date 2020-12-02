#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Level.h"

using namespace std;
using namespace glm;


Level::Level(int id)
	: levelID(id)
{
	this->World::World();

	loadShaders();
	loadModels();
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
	//this->World::render();

	renderTileMap();
	// render Ball
	// render Slides
}

void Level::renderTileMap() const
{
	Model* model;
	ShaderProgram* shader;
	bool skip = false;

	for (const vector<Tile>& row : map)
	{
		for (const Tile& tile : row)
		{
			if (tile.isUndefined()) continue;

			switch (tile.type)
			{
			case Tile::cube:
				shader	= cubeShader;
				model	= cubeModel;
				break;

			default:
				shader	= nullptr;
				model	= nullptr;
				break;
			}

			// Compute ModelMatrix
			mat4 modelMatrix = mat4(1.0f);
			modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
			modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
			modelMatrix = translate(modelMatrix, -model->getCenter());
			
			// Compute NormalMatrix
			mat3 normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));
			
			// Set uniforms
			shader->use();
			shader->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			shader->setUniformMatrix4f("projection", projMatrix);
			shader->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
			shader->setUniformMatrix3f("normalmatrix", normalMatrix);
			
			// Render
			model->render(*shader);
		}
	}

}

int Level::getTileSize()
{
	return tileSize;
}

ivec2 Level::getChunkSize()
{
	return chunkSize;
}

ivec2 Level::getMapSizeInTiles()
{
	return mapSizeInTiles;
}

ivec2 Level::getMapSizeInChunks()
{
	return mapSizeInChunks;
}

vec2 Level::getTopLeftCornerCoordsOfChunk(int i, int j)
{
	 // not implemented yet
	float tileSize = float(this->getTileSize());
	vec2 chunkSizeInTiles = vec2(this->getChunkSize());
	vec2 chunkSizeInPixels = tileSize * chunkSizeInTiles;
	vec2 chunkCentreDisp = chunkSizeInPixels / 2.f;

	vec2 chunkCoords = vec2(0.f, -(tileSize / 2.f));

	return vec2();
}

Tile* Level::getFirstTileOfChunk(int k)
{
	return firstTileOfChunk[k];
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
	map					= vector<vector<Tile>>(mapSizeInTiles.y, vector<Tile>(mapSizeInTiles.x));
	firstTileOfChunk	= vector<Tile*>(mapSizeInChunks.x * mapSizeInChunks.y);

	getline(fin, line); // Blank line

	Tile* currentTile;
	int currentChunk = -1;
	for (int i = 0; i < mapSizeInTiles.y; i++)
	{
		for (int j = 0; j < mapSizeInTiles.x; j++)
		{
			fin.get(tile);

			currentTile = loadTile(tile, i, j);


			if (currentTile->chunk > currentChunk)
			{
				currentChunk = currentTile->chunk;
				firstTileOfChunk[currentChunk] = currentTile;
			}
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

Tile* Level::loadTile(char type, int i, int j)
{
	Tile& tile = map[i][j];

	int		chunk	= mapSizeInChunks.x * (i / chunkSize.y) + (j / chunkSize.x);
	vec2	coords	= float(tileSize) * vec2(float(j) + 0.5f, -(float(i) + 0.5f));

	switch (type)
	{

	case Tile::cube:		tile = Tile(coords, chunk, type, true, false);	break;
	case Tile::undefined:	tile = Tile(coords, chunk, type, false, false);	break;

	default: break;
	}

	return &map[i][j];
}

void Level::loadModels()
{
	cubeModel = new Model();
	cubeModel->loadFromFile("models/cube.obj", *cubeShader);
}

#include "Scene.h"

void Level::loadShaders()
{
	cubeShader = new ShaderProgram();
	Scene::loadShaders("cubeShader", cubeShader);

}





























