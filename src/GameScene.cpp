#include "GameScene.h"

void GameScene::init()
{
	this->Scene::init();

	currentChunk = 0;
	level = new Level(this, 1);
	level->setViewMatrix(view);
	level->setProjMatrix(projection);

	initBall();
}

void GameScene::render()
{

	if (cam->isFree())
		view = cam->getViewMatrix();
	else
		view = lookAtCurrentChunk();
	

	renderAxis();

	//world->setViewMatrix(view);
	//world->setProjMatrix(projection);
	//world->render();

	//defaultShaderProgram->use();
	//defaultShaderProgram->setUniformMatrix4f("projection", projection);
	//defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	//defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix);
	//defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);


	level->Level::render();


	ball->Ball::render();


}

void GameScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	ball->Ball::update(deltaTime);

	level->update(deltaTime);

	ent->Slide::update(deltaTime);

	ball->Ball::update(deltaTime);
	updateCurrentChunk();
	checkCollisionsAndUpdateEntitiesPositions(deltaTime);
}

ivec2 GameScene::toTileCoords(vec2 coords)
{
	float tileSize = float(level->getTileSize());

	return ivec2(coords/tileSize) * ivec2(1,-1);
}

ivec2 GameScene::toTileCoordsNotInverting(vec2 coords)
{
	float tileSize = float(level->getTileSize());

	return ivec2(coords / tileSize);
}

bool GameScene::checkCollision_Ball_World(int time)
{
	vec2 newBallPosition = ball->Ball::getPosition() + (float(time / 100.f) * ball->Ball::getDirection() * ball->Ball::getSpeed());

	ball->Ball::setPosition(newBallPosition);

	contourTileList ballContourTileList = ball->listOfContourTiles();

	int upCount = 0, downCount = 0, rightCount = 0, leftCount = 0;

	for (Tile* tile : ballContourTileList.down)  if (tile->solid) downCount++;
	for (Tile* tile : ballContourTileList.up)	 if (tile->solid) upCount++;
	for (Tile* tile : ballContourTileList.right) if (tile->solid) rightCount++;
	for (Tile* tile : ballContourTileList.left)  if (tile->solid) leftCount++;

	int totalCount = upCount + downCount + rightCount + leftCount;
	bool onSolid = totalCount > 0;

	if (onSolid)
	{
		vec2 displacement = (float(time / 100.f) * ball->Ball::getSpeed());

		if (downCount > 1 && downCount > leftCount && downCount > rightCount)
		{
			ball->Ball::setPosition(ball->Ball::getPosition() + vec2(0.f, displacement.y));
			ball->invertDirectionY();
		}
		else if (upCount > 1 && upCount > leftCount && upCount > rightCount)
		{
			ball->Ball::setPosition(ball->Ball::getPosition() + vec2(0.f, -displacement.y));
			ball->invertDirectionY();
		}

		if (leftCount > 1 && leftCount > downCount && leftCount > upCount)
		{
			ball->Ball::setPosition(ball->Ball::getPosition() + vec2(displacement.x, 0.f));
			ball->invertDirectionX();
		}
		else if (rightCount > 1 && rightCount > downCount && rightCount > upCount)
		{
			ball->Ball::setPosition(ball->Ball::getPosition() + vec2(-displacement.x, 0.f));
			ball->invertDirectionX();
		}

		return true;
	}

	return false;
}

bool GameScene::checkCollision_Slide_World(Slide* slide, int time)
{
	bool slideCollision = false;
	vec2 oldSlidePosition = slide->Slide::getPosition();
	vec2 newSlidePosition = oldSlidePosition + (float(time / 100.f) * slide->Slide::getDirection() * slide->Slide::getSpeed());

	slide->setPosition(newSlidePosition);

	vector<vector<ivec2>> tiles = slide->Slide::occupiedTiles();
	
	for (auto row : tiles)
	{
		if (level->getTile(row[0])->solid)
		{
			slide->Slide::setPosition(oldSlidePosition);

			if (slide->isVertical())	slide->invertDirectionY();
			else /* horizontal*/		slide->invertDirectionX();

			return true;
		}
	}

	return false;
}

void GameScene::checkCollisionsAndUpdateEntitiesPositions(int deltaTime)
{	
	bool ballCollidedWithWorld = false;
	
	int time, step;
	for (time = step = 1; time <= deltaTime; time += step)
	{
		ballCollidedWithWorld = checkCollision_Ball_World(time);

		bool slideCollidedWorld;
		for (Slide* slide : level->getSlides())
		{
			slideCollidedWorld = checkCollision_Slide_World(slide, time);
		}

		// En este punto del codigo, ni la pelota ni las palas estan chocando con el mundo.
		
	}
}

void GameScene::updateCurrentChunk()
{
	currentChunk = level->getTile(toTileCoords(ball->getPosition()))->chunk;
}

mat4 GameScene::lookAtCurrentChunk()
{
	vec2 distanceToChunkCentre, chunkCentre;

							// TileSize * (chunkSize - 1) / 2   ---->  El 1 esta para desplazar medio cubo extra
	distanceToChunkCentre = float(level->getTileSize()) * (vec2(level->getChunkSize()) - vec2(1.f)) / 2.f;
	distanceToChunkCentre = vec2(1, -1) * distanceToChunkCentre;
	chunkCentre           = level->getFirstTileOfChunk(currentChunk)->coords + distanceToChunkCentre;

	float zDisplacement = float(level->getTileSize()) * (float(glm::max(level->getChunkSize().x, level->getChunkSize().y)) + 1.f) / 2.f;

	return lookAt(vec3(chunkCentre, zDisplacement), vec3(chunkCentre, 0.f), vec3(0.f, 1.f, 0.f));
}

void GameScene::addCube()
{
	// YA NO HAY NADA
}

void GameScene::initBall()
{
	// Load Shader
	ballShader = new ShaderProgram();
	loadShaders("ballShader", ballShader);

	// Load Model
	ballModel = new Model();
	ballModel->loadFromFile("models/cube.obj", (*ballShader));

	// Create Entity
	ball = new Ball(this, ballModel, ballShader);

	ball->Ball::init();
	ball->setViewMatrix(view);
	ball->setProjMatrix(projection);
}

void GameScene::playerPressedSpace()
{
	ball->invertDirectionY();
}

Level* GameScene::getLevel()
{
	return level;
}

vec2 GameScene::getPlayerPos()
{
	return ball->getPosition();
}

vec2 GameScene::getPlayerSpd()
{
	return ball->getSpeed();
}

vec2 GameScene::getPlayerDir()
{
	return ball->getDirection();
}
