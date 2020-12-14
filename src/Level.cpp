#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Level.h"
#include "GameScene.h"
#include "Game.h"

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

void Level::init()
{
	finishWallChecks();
	extrudeAllTheSnakeDoors();
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
	else
		trail->clear();

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
				modelMatrix = scale(modelMatrix, vec3(0.5f * float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::opened_snake_door:
				shader = cubeShader;
				model = snakeOpenedDoorModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::closed_snake_door:
				shader = cubeShader;
				model = snakeClosedDoorModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0.f));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::wallCheckVR:
			case Tile::wallCheckVL:
			case Tile::wallCheckHR:
			case Tile::wallCheckHL:
				shader = cubeShader;
				model = cubeModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, -tileSize));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::switchable_solid:
				shader = cubeShader;
				model = snakeClosedDoorModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::switchable_no_solid:
				shader = cubeShader;
				model  = voidCubeModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;



			case Tile::buttonR_ON:
				shader = cubeShader;
				model = buttonONModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;

			case Tile::buttonL_ON:
				shader = cubeShader;
				model = buttonONModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;
			case Tile::buttonU_ON:
				shader = cubeShader;
				model = buttonONModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;
			case Tile::buttonD_ON:
				shader = cubeShader;
				model = buttonONModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;


			case Tile::buttonR_OFF:
				shader = cubeShader;
				model = buttonOFFModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;
			case Tile::buttonL_OFF:
				shader = cubeShader;
				model = buttonOFFModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;
			case Tile::buttonU_OFF:
				shader = cubeShader;
				model = buttonOFFModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
				modelMatrix = scale(modelMatrix, vec3(float(tileSize) / model->getHeight()));
				modelMatrix = translate(modelMatrix, -model->getCenter());
				break;
			case Tile::buttonD_OFF:
				shader = cubeShader;
				model = buttonOFFModel;
				modelMatrix = translate(modelMatrix, vec3(tile.coords, 0));
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

void Level::closeAllSnakeDoors()
{
	auto& openedDoor_it = snake_opened_door_list.begin();

	while (!snake_opened_door_list.empty())
	{
		Door* closedDoor = *openedDoor_it;

		for (Tile* doorTile : *closedDoor)
		{
			ivec2 tileJI = scene->toTileCoords(doorTile->coords);
			loadTile(Tile::closed_snake_door, tileJI[1], tileJI[0]);
		}

		snake_closed_door_list.push_back(closedDoor);
		openedDoor_it = snake_opened_door_list.erase(openedDoor_it);
	}
}

void Level::extrudeAllTheSnakeDoors()
{
	vector<Tile*> listOfStartingTiles(0);

	for (const vector<Tile>& row : map)
		for (const Tile& tile : row)
			if (tile.type == Tile::closed_snake_door)
				listOfStartingTiles.push_back((Tile*) &tile);

	for (Tile* tile: listOfStartingTiles)
		addSnakeClosedDoor(tile);
}

void Level::addSnakeClosedDoor(Tile* startingTile)
{
	Door* closed_door = new Door(0);

	Tile* currentTile = startingTile;
	ivec2 tileCoords  = scene->toTileCoords(startingTile->coords);

	// j,i
	do
	{
		currentTile = loadTile(Tile::closed_snake_door, tileCoords[1], tileCoords[0]);
		closed_door->push_back(currentTile);

		tileCoords += ivec2(0, 1);
		currentTile = getTile(tileCoords);
	} 
	while (!currentTile->solid);

	snake_closed_door_list.push_back(closed_door);
}

void Level::openThisSnakeDoor(Tile* tile)
{
	Door* doorToOpen = nullptr;

	for (auto closedDoor_it = snake_closed_door_list.begin(); closedDoor_it != snake_closed_door_list.end(); )
	{
		Door* closedDoor = *closedDoor_it;

		for (Tile* doorTile : *closedDoor)
		{
			if (doorTile == tile)
			{
				doorToOpen = closedDoor;
				break;
			}
		}

		if (doorToOpen != nullptr)
		{
			// Borramos la puerta a abrir de la lista de puertas cerradas
			closedDoor_it = snake_closed_door_list.erase(closedDoor_it);
			break;
		}
	}

	//  Si la puerta a abrir no existe, retorna;
	if (doorToOpen == nullptr) return;

	// Abrimos todas las tiles de la puerta a abrir
	for (Tile* doorTile : *doorToOpen)
	{
		ivec2 tileJI = scene->toTileCoords(doorTile->coords);
		loadTile(Tile::opened_snake_door, tileJI[1], tileJI[0]);
	}

	// Anadimos la futura puerta abierta a la lista de puertas abiertas
	snake_opened_door_list.push_back(doorToOpen);

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

void Level::checkTrigger(ivec2 tilePos)
{
	for (auto wallChk : wallChecks) {
		if (wallChk->triggerInRange(tilePos))
			return;
	}
}

// PURE TESTING REMOVE LATER
void Level::changeSpawnPoint()
{
	/*if (spawns.size() > 1)
		spawns[1]->collided();*/
	for (auto wallChk : wallChecks) {
		wallChk->godTrigger();
	}
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
			
			if (isAButton(currentTile))
			{
				buttons.push_back(currentTile);
			}

			if (isASwitchableTile(currentTile))
			{
				switchableTiles.push_back(ivec2(j, i));
			}

			if (currentTile->isUndefined()) // Dynamic Tile
			{
				switch (tile)
				{
				case Tile::verticalSlideChase:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init(tileSize, Slide::vertical, currentTile->coords, vec2(0.f, 1.f), vec2(0.f, 0.5f), Slide::chase);
					slides.push_back(slide);
					break;
				}
				case Tile::horizontalSlideChase:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init(tileSize, Slide::horizontal, currentTile->coords, vec2(1.f, 0.f), vec2(0.5f, 0.f), Slide::chase);
					slides.push_back(slide);
					break;
				}
				case Tile::verticalSlideEscape:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init(tileSize, Slide::vertical, currentTile->coords, vec2(0.f, 1.f), vec2(0.f, 0.5f), Slide::escape);
					slides.push_back(slide);
					break;
				}
				case Tile::horizontalSlideEscape:
				{
					Slide* slide = new Slide(scene, slideModel, slideShader);
					slide->Slide::init(tileSize, Slide::horizontal, currentTile->coords, vec2(1.f, 0.f), vec2(0.5f, 0.f), Slide::escape);
					slides.push_back(slide);
					break;
				}
				case Tile::spawnPoint:
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
				case Tile::wallCheckVR:
				case Tile::wallCheckVL:
				case Tile::wallCheckHR:
				case Tile::wallCheckHL:
				{
					WallCheckpoint* aux = new WallCheckpoint(this,tile);
					ivec2 pos = ivec2(currentTile->coords / float(tileSize)) * ivec2(1, -1);

					aux->addWall(pos);

					if(tile == Tile::wallCheckVL)
						pos += ivec2(+2, 0);
					else if(Tile::wallCheckVR)
						pos += ivec2(2, 0);
					else if (Tile::wallCheckHR)
						pos += ivec2(0, 2);
					else if (Tile::wallCheckHL)
						pos += ivec2(0, -2);

					aux->addTrigger(pos);

					wallChecks.push_back(aux);

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

void Level::finishWallChecks()
{
	for (int k = 0; k < wallChecks.size();++k) {
		if (wallChecks[k]->getType() == Tile::wallCheckVR)
			fillVerticalRight(wallChecks[k]);
		else if (wallChecks[k]->getType() == Tile::wallCheckVL)
			fillVerticalLeft(wallChecks[k]);
		else if (wallChecks[k]->getType() == Tile::wallCheckHR)
			fillHorizontalRight(wallChecks[k]);
		else if (wallChecks[k]->getType() == Tile::wallCheckHL)
			fillHorizontalLeft(wallChecks[k]);
	}
}

void Level::toggleButton(Tile* button)
{
	ivec2 tileCoords = scene->toTileCoords(button->coords);

	char newButtonType;

	switch (button->type)
	{
	case Tile::buttonR_ON: newButtonType = Tile::buttonR_OFF; break;
	case Tile::buttonL_ON: newButtonType = Tile::buttonL_OFF; break;
	case Tile::buttonU_ON: newButtonType = Tile::buttonU_OFF; break;
	case Tile::buttonD_ON: newButtonType = Tile::buttonD_OFF; break;

	case Tile::buttonR_OFF: newButtonType = Tile::buttonR_ON; break;
	case Tile::buttonL_OFF: newButtonType = Tile::buttonL_ON; break;
	case Tile::buttonU_OFF: newButtonType = Tile::buttonU_ON; break;
	case Tile::buttonD_OFF: newButtonType = Tile::buttonD_ON; break;
	}

	loadTile(newButtonType, tileCoords.y, tileCoords.x);
}

void Level::touchButton(Tile* button)
{
	if (buttonIsPressed(button)) return;

	for (Tile* button : buttons)
		toggleButton(button);

	for (ivec2 tileCoords : switchableTiles)
	{
		Tile* switchable = getTile(tileCoords);
		
		switch (switchable->type)
		{
		case Tile::switchable_no_solid:
			loadTile(Tile::switchable_solid, tileCoords.y, tileCoords.x);
			break;
		case Tile::switchable_solid:
			loadTile(Tile::switchable_no_solid, tileCoords.y, tileCoords.x);
			break;
		}
	}

	Game::instance().playButtonSound();
	Game::instance().playDoorSound();
}

bool Level::isASwitchableTile(Tile* tile)
{
	switch (tile->type)
	{
	case Tile::switchable_no_solid:
	case Tile::switchable_solid:
		return true;
	}

	return false;
}

bool Level::isAButton(Tile* tile)
{
	switch (tile->type)
	{
	case Tile::buttonR_ON:
	case Tile::buttonL_ON:
	case Tile::buttonU_ON:
	case Tile::buttonD_ON:
	case Tile::buttonR_OFF:
	case Tile::buttonL_OFF:
	case Tile::buttonU_OFF:
	case Tile::buttonD_OFF:
		return true;
	}

	return false;
}

bool Level::buttonIsPressed(Tile* tile)
{
	switch (tile->type)
	{
	case Tile::buttonR_ON:
	case Tile::buttonL_ON:
	case Tile::buttonU_ON:
	case Tile::buttonD_ON:
		return true;
	}

	return false;
}

void Level::fillVerticalRight(WallCheckpoint* wall)
{
	Tile* currTile = getTile(wall->getWall(0));
	ivec2 pos = scene->toTileCoords(currTile->coords);
	// j,i
	while (!currTile->solid) {
		pos += ivec2(0,1);
		wall->addWall(pos);
		wall->addTrigger(pos + ivec2(2, 0));
		currTile->type = Tile::wallCheckVR;
		currTile = getTile(pos);
	}
}

void Level::fillVerticalLeft(WallCheckpoint* wall)
{
	Tile* currTile = getTile(wall->getWall(0));
	ivec2 pos = scene->toTileCoords(currTile->coords);
	// j,i
	while (!currTile->solid) {
		pos += ivec2(0, 1);
		wall->addWall(pos);
		wall->addTrigger(pos + ivec2(-2, 0));
		currTile->type = Tile::wallCheckVL;
		currTile = getTile(pos);
	}
}

void Level::fillHorizontalRight(WallCheckpoint* wall)
{
	Tile* currTile = getTile(wall->getWall(0));
	ivec2 pos = scene->toTileCoords(currTile->coords);
	// j,i
	while (!currTile->solid) {
		pos += ivec2(1, 0);
		wall->addWall(pos);
		wall->addTrigger(pos + ivec2(0, 2));
		currTile->type = Tile::wallCheckHR;
		currTile = getTile(pos);
	}
}

void Level::fillHorizontalLeft(WallCheckpoint* wall)
{
	
	Tile* currTile = getTile(wall->getWall(0));
	ivec2 pos = scene->toTileCoords(currTile->coords);
	// j,i
	while (!currTile->solid) {
		pos += ivec2(1, 0);
		wall->addWall(pos);
		wall->addTrigger(pos + ivec2(0, -2));
		currTile->type = Tile::wallCheckHL;
		currTile = getTile(pos);
	}
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

	case Tile::closed_snake_door:
		tile = Tile(coords, chunk, type, true, false);
		return &map[i][j];

	case Tile::opened_snake_door:
		tile = Tile(coords, chunk, type, false, false);
		return &map[i][j];

	case Tile::switchable_no_solid:
		tile = Tile(coords, chunk, type, false, false);
		return &map[i][j];

	case Tile::switchable_solid:
		tile = Tile(coords, chunk, type, true, false);
		return &map[i][j];

	case Tile::buttonR_ON :
	case Tile::buttonL_ON :
	case Tile::buttonU_ON :
	case Tile::buttonD_ON :
	case Tile::buttonR_OFF:
	case Tile::buttonL_OFF:
	case Tile::buttonU_OFF:
	case Tile::buttonD_OFF:
		tile = Tile(coords, chunk, type, true, false);
		return &map[i][j];

	default: break;
	}

	tile = Tile(coords, chunk, Tile::undefined, false, false);
	return &map[i][j];
}

void Level::loadModels()
{
	voidCubeModel = new Model();
	voidCubeModel->loadFromFile("models/noSolidCube.obj", *cubeShader);

	snakeModel = new Model();
	snakeModel->loadFromFile("models/snake.obj", *cubeShader);

	snakeClosedDoorModel = new Model();
	snakeClosedDoorModel->loadFromFile("models/snakeDoor.obj", *cubeShader);

	snakeOpenedDoorModel = new Model();
	snakeOpenedDoorModel->loadFromFile("models/noSolidCube.obj", *cubeShader);

	ropeModel = new Model();
	ropeModel->loadFromFile("models/rope.obj", *ropeShader);

	cubeModel = new Model();
	cubeModel->loadFromFile("models/cubeBlue.obj", *cubeShader);

	pinchoModel = new Model();
	pinchoModel->loadFromFile("models/pincho.obj", *cubeShader);
	
	bolaPinchoModel = new Model();
	bolaPinchoModel->loadFromFile("models/UPC.obj", *cubeShader);

	buttonONModel = new Model();
	buttonONModel->loadFromFile("models/button_ON.obj", *buttonShader);

	buttonOFFModel = new Model();
	buttonOFFModel->loadFromFile("models/button_OFF.obj", *buttonShader);
}

#include "Scene.h"

void Level::loadShaders()
{
	cubeShader = new ShaderProgram();
	Scene::loadShaders("cubeShader", cubeShader);

	ropeShader = new ShaderProgram();
	Scene::loadShaders("ropeShader", ropeShader);

	buttonShader = new ShaderProgram();
	Scene::loadShaders("buttonShader", buttonShader);
}





























