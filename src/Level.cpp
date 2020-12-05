#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Level.h"
#include "GameScene.h"

using namespace std;
using namespace glm;


Level::Level(GameScene* scene, int id)
	: levelID(id), scene(scene)
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

void Level::update(int deltaTime)
{
	for (auto* slide : slides)
		slide->Slide::update(deltaTime);
}

void Level::render() const
{
	//this->World::render();

	renderTileMap();
	renderSlides();
}

void Level::renderSlides() const
{
	for (auto* slide : slides)
	{
		slide->setViewMatrix(viewMatrix);
		slide->setProjMatrix(projMatrix);
		slide->Slide::render();
	}
}

void Level::renderTileMap() const
{
	Model* model;
	ShaderProgram* shader;
	bool skip = false;

	mat4 modelMatrix;
	mat3 normalMatrix;

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
			modelMatrix = mat4(1.0f);
			modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
			modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
			modelMatrix = translate(modelMatrix, -model->getCenter());
			
			// Compute NormalMatrix
			normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));
			
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

Tile* Level::getTile(ivec2 coords)
{
	return getTile(coords.y, coords.x);
}

Tile* Level::getTile(int i, int j)
{
	return &map[i][j];
}

Tile* Level::getFirstTileOfChunk(int k)
{
	return firstTileOfChunk[k];
}

vector<Slide*>& Level::getSlides()
{
	return slides;
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


	//// LOAD HORIZONTAL SLIDES
	//getline(fin, line);	// Blank line
	//getline(fin, line);
	//if (diff(line, "START  - HORIZONTAL SLIDES"))
	//	throw std::string("Error parsing " + line).c_str();
	//
	//getline(fin, line);
	//while (diff(line, "END    - HORIZONTAL SLIDES"))
	//{
	//	sstream.str(line);
	//	sstream >> i >> j0 >> j1;
	//	Slide* slide = new Slide(scene, slideModel, slideShader);
	//	slide->Slide::init();
	//	slide->setSize(tileSize, Slide::horizontal);
	//	slide->setLimits(j0, j1);
	//	slides.push_back(slide);
	//	getline(fin, line);
	//}


	//// LOAD VERTICAL SLIDES
	//getline(fin, line); // Blank line
	//getline(fin, line);
	//if (diff(line, "START  - VERTICAL SLIDES"))
	//	throw std::string("Error parsing " + line).c_str();
	//getline(fin, line);
	//while (diff(line, "END    - VERTICAL SLIDES"))
	//{
	//	sstream.str(line);
	//	sstream >> j >> i0 >> i1;
	//	Slide* slide = new Slide(scene, slideModel, slideShader);
	//	slide->Slide::init();
	//	slide->setSize(10, Slide::vertical);
	//	slide->setLimits(i0, i1);
	//	slides.push_back(slide);
	//	getline(fin, line);
	//}
	

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


	// LOAD SLIDE MODEL AND SHADER
	ShaderProgram* slideShader = new ShaderProgram();
	Model* slideModel = new Model();
	Scene::loadShaders("ballShader", slideShader);
	slideModel->loadFromFile("models/cube.obj", (*slideShader));


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
			
			if (currentTile != nullptr) // Static Tiles
			{
				if (currentTile->chunk > currentChunk)
				{
					currentChunk = currentTile->chunk;
					firstTileOfChunk[currentChunk] = currentTile;
				}
			}
			else // Dynamic Tile
			{
				switch (tile)
				{
				case verticalSlide:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init();
					slide->setSize(tileSize, Slide::vertical);
					slide->setPosition(float(tileSize) * vec2(float(j) + 0.5f, -(float(i) + 0.5f)));
					slide->setDirection(vec2(0.f, 1.f));
					slide->setSpeed(vec2(0.f, 0.5f));
					slides.push_back(slide);
					break;
				}
				case horizontalSlide:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init();
					slide->setSize(tileSize, Slide::horizontal);
					slide->setPosition(float(tileSize) * vec2(float(j) + 0.5f, -(float(i) + 0.5f)));
					slide->setDirection(vec2(1.f, 0.f));
					slide->setSpeed(vec2(0.5f, 0.f));
					slides.push_back(slide);
					break;
				}
				default: break;
				}
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
	
	case blank:
	case Tile::undefined:	tile = Tile(coords, chunk, Tile::undefined, false, false);	break;

	default: return nullptr;
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





























