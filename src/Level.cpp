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

	onTrail = false;
	trail = new Trail(scene);
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
	this->setProjMatrix(scene->getProjMatrix());
	this->setViewMatrix(scene->getViewMatrix());

	if (onTrail)
		trail->update(deltaTime);

	for (auto* slide : slides)
		slide->Slide::update(deltaTime);
}

void Level::render() const
{
	//this->World::render();

	renderTileMap();
	renderSlides();
	renderSpawns();
	
	if (onTrail)
		trail->render();
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

			modelMatrix = mat4(1.0f);

			switch (tile.type)
			{
			case Tile::cube:
				shader		= cubeShader;
				model		= cubeModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::ropeH:
				shader		= ropeShader;
				model		= ropeModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / (model->getHeight())));
				modelMatrix = rotate(modelMatrix, PI/2.f, vec3(0.f, 0.f, 1.f));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::ropeV:
				shader		= ropeShader;
				model		= ropeModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / (model->getHeight())));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::pinchoU:
				shader		= cubeShader;
				model		= pinchoModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::pinchoD:
				shader		= cubeShader;
				model		= pinchoModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = rotate(modelMatrix, PI, vec3(0.f, 0.f, 1.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::pinchoR:
				shader		= cubeShader;
				model		= pinchoModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = rotate(modelMatrix, -PI/2.f, vec3(0.f,0.f,1.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::pinchoL:
				shader		= cubeShader;
				model		= pinchoModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = rotate(modelMatrix, PI / 2.f, vec3(0.f, 0.f, 1.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::winTile:
				shader		= cubeShader;
				model		= cubeModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;
				
			case Tile::snake:
				shader		= cubeShader;
				model		= snakeModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			default:
				shader	= nullptr;
				model	= nullptr;
				break;
			}
			
			// Compute NormalMatrix
			normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));
			
			// Set uniforms
			shader->use();
			shader->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			shader->setUniformMatrix4f("projection", mat4(projMatrix));
			shader->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
			shader->setUniformMatrix3f("normalmatrix", normalMatrix);
			
			// Render
			model->render(*shader);
		}
	}
}

void Level::renderSpawns() const
{
	for (auto* spawn : spawns)
	{
		spawn->setViewMatrix(viewMatrix);
		spawn->setProjMatrix(projMatrix);
		spawn->SpawnCheckpoint::render();
	}
}

void Level::setTrail(bool start)
{
	if (!start) trail->clear();

	onTrail = start;
}

bool Level::ballIsOnTrail()
{
	return onTrail;
}

bool Level::checkIfBallCollidedWithTrail()
{
	return trail->ballCollidedWithTrail();
}

void Level::addPointToTrail(vec2 pos)
{
	trail->addPoint(pos);
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

Slide* Level::whichSlideIsCollidingWithTheBall()
{
	for (Slide* slide : slides)
		if (scene->ballAndSlideAreColliding(slide))
			return slide;

	return nullptr;
}

vector<Slide*>& Level::getSlides()
{
	return slides;
}

void Level::checkIfBallCollidedWithACheckpoint()
{
	ivec2 ballTileCoords = getTile(scene->toTileCoords(scene->getPlayerPos()))->coords;

	for (SpawnCheckpoint* spawn : spawns)
	{
		ivec2 spawnTileCoords = getTile(scene->toTileCoords(spawn->getPosition()))->coords;

		if (ballTileCoords == spawnTileCoords)
		{
			spawn->collided();
			return;
		}
	}
}

void Level::addSpawnPoint(vec2 pos)
{
	spawnPositions.push(pos);
	scene->setSpawnPoint(spawnPositions.top());
}

void Level::removeSpawnPoint()
{
	if (spawnPositions.size() > 1) {
		spawnPositions.pop();
		scene->setSpawnPoint(spawnPositions.top());
	}
}

// PURE TESTING REMOVE LATER
void Level::changeSpawnPoint()
{
	if (spawns.size() > 1)
		spawns[1]->collided();
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
	std::getline(fin, line);
	if (diff(line, "LEVEL")) throw "LEVEL NOT FOUND";
	std::getline(fin, line);	// Blank line



	// LOAD LEVEL PARAMETERS
	std::getline(fin, line);	// Blank line
	std::getline(fin, line);
	if (diff(line, "START  - TILEMAP"))
		throw std::string("Error parsing " + line).c_str();
	std::getline(fin, line);
	sstream.str(line);
	sstream >> tileSize;




	std::getline(fin, line);
	sstream.str(line);
	sstream >> chunkSize.y >> chunkSize.x;

	std::getline(fin, line);
	sstream.str(line);
	sstream >> mapSizeInChunks.y >> mapSizeInChunks.x;

	mapSizeInTiles = mapSizeInChunks * chunkSize;


	// LOAD SLIDE MODEL AND SHADER
	ShaderProgram* slideShader = new ShaderProgram();
	Model* slideModel = new Model();
	Scene::loadShaders("slideShader", slideShader);
	slideModel->loadFromFile("models/cubeBlue.obj", (*slideShader));


	// INICIALIZAR Y LEER TILE MAP
	char tile;
	map					= vector<vector<Tile>>(mapSizeInTiles.y, vector<Tile>(mapSizeInTiles.x));
	firstTileOfChunk	= vector<Tile*>(mapSizeInChunks.x * mapSizeInChunks.y);
	std::getline(fin, line); // Blank line

	bool firstSpawnPoint = true;

	Tile* currentTile;
	int lastSavedChunk = -1;
	for (int i = 0; i < mapSizeInTiles.y; i++)
	{
		for (int j = 0; j < mapSizeInTiles.x; j++)
		{
			fin.get(tile);

			currentTile = loadTile(tile, i, j);
			
			if (currentTile->chunk > lastSavedChunk)
			{
				lastSavedChunk = currentTile->chunk;
				firstTileOfChunk[lastSavedChunk] = currentTile;
			}
			
			if (currentTile->isUndefined()) // Dynamic Tile
			{
				switch (tile)
				{
				case verticalSlideChase:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init(tileSize, Slide::vertical, currentTile->coords, vec2(0.f, 1.f), vec2(0.f, 0.5f), Slide::chase);
					slides.push_back(slide);
					break;
				}
				case horizontalSlideChase:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init(tileSize, Slide::horizontal, currentTile->coords, vec2(1.f, 0.f), vec2(0.5f, 0.f), Slide::chase);
					slides.push_back(slide);
					break;
				}
				case verticalSlideEscape:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init(tileSize, Slide::vertical, currentTile->coords, vec2(0.f, 1.f), vec2(0.f, 0.5f), Slide::escape);
					slides.push_back(slide);
					break;
				}
				case horizontalSlideEscape:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init(tileSize, Slide::horizontal, currentTile->coords, vec2(1.f, 0.f), vec2(0.5f, 0.f), Slide::escape);
					slides.push_back(slide);
					break;
				}
				case spawnPoint:
				{
					SpawnCheckpoint* aux = new SpawnCheckpoint(scene,slideModel,slideShader);
					aux->init();
					
					startPoint = currentTile->coords;

					aux->setPosition(startPoint);

					spawns.push_back(aux);

					if (firstSpawnPoint) {
						spawnPositions.push(startPoint);
						scene->setSpawnPoint(spawnPositions.top());
						scene->locateBallInSpawnPoint();
						firstSpawnPoint = false;
					}

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

	std::getline(fin, line); // Blank line
	std::getline(fin, line);
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
	case Tile::cube:
		tile = Tile(coords, chunk, type, true, false);
		return &map[i][j];

	case Tile::ropeH:
		tile = Tile(coords, chunk, type, false, false);
		return &map[i][j];
	
	case Tile::ropeV:
		tile = Tile(coords, chunk, type, false, false);
		return &map[i][j];

	case Tile::pinchoU:
	case Tile::pinchoD:
	case Tile::pinchoR:
	case Tile::pinchoL:
		tile = Tile(coords, chunk, type, true, true);
		return &map[i][j];
	
	case Tile::winTile:
		winPoint = coords;
		tile = Tile(coords, chunk, type, false, false);
		return &map[i][j];

	case Tile::snake:
		tile = Tile(coords, chunk, type, false, false);
		return &map[i][j];

	default: break;
	}

	tile = Tile(coords, chunk, Tile::undefined, false, false);
	return &map[i][j];
}

void Level::loadModels()
{
	voidCubeModel = new Model();
	voidCubeModel->loadFromFile("models/voidCube.obj", *cubeShader);

	snakeModel = new Model();
	snakeModel->loadFromFile("models/snake.obj", *cubeShader);

	ropeModel = new Model();
	ropeModel->loadFromFile("models/rope.obj", *ropeShader);

	cubeModel = new Model();
	cubeModel->loadFromFile("models/cubeBlue.obj", *cubeShader);

	pinchoModel = new Model();
	pinchoModel->loadFromFile("models/pincho.obj", *cubeShader);
	
	bolaPinchoModel = new Model();
	bolaPinchoModel->loadFromFile("models/UPC.obj", *cubeShader);

	buttonONModel = new Model();
	buttonONModel->loadFromFile("model/buttonOn.obj", *buttonShader);

	buttonOFFModel = new Model();
	buttonOFFModel->loadFromFile("model/buttonOn.obj", *buttonShader);
}

#include "Scene.h"

void Level::loadShaders()
{
	cubeShader = new ShaderProgram();
	Scene::loadShaders("cubeShader", cubeShader);

	ropeShader = new ShaderProgram();
	Scene::loadShaders("ropeShader", ropeShader);

	buttonShader = new ShaderProgram();
	Scene::loadShaders("buttonShader", ropeShader);
}





























