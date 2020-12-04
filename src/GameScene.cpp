#include "GameScene.h"

void GameScene::init()
{
	this->Scene::init();

	models.resize(1, new Model());

	for (Model* model : models)
		model->loadFromFile("models/sphere.obj", (*defaultShaderProgram));

	currentChunk = 0;
	level = new Level(this, 1);

	initBall();

	// TOMEU TESTING
	auxMod = new Model();
	auxMod->loadFromFile("models/cube.obj", (*defaultShaderProgram));
	ent = new Slide(this, auxMod, defaultShaderProgram);
	ent->Slide::init();
	ent->setSize(level->getTileSize(), 1);
	ent->setLimits(5, 5);
}

void GameScene::render()
{
	float     scaleFactor;
	glm::vec3 centerModelBase;
	glm::vec3 transl = vec3(0);
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	glm::mat4 viewMatrix = cam->getViewMatrix();

	if (!cam->isFree())
		viewMatrix = lookAtCurrentChunk();

	renderAxis();

	world->setViewMatrix(viewMatrix);
	world->setProjMatrix(projection);
	//world->render();

	float ratio;
	for (Model* model : models)
	{
		ratio = model->getHeight();
		scaleFactor = ratio / model->getHeight();

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
		modelMatrix = glm::translate(modelMatrix, -model->getCenter());
		modelMatrix = glm::translate(modelMatrix, transl);

		// Compute Normal Matrix

		normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

		defaultShaderProgram->use();
		defaultShaderProgram->setUniformMatrix4f("projection", projection);
		defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
		defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);

		model->render(*defaultShaderProgram);
		transl = transl + vec3(ratio,0,0);
	}

	ent->setViewMatrix(viewMatrix);
	ent->setProjMatrix(projection);
	ent->Slide::render();

	level->setViewMatrix(viewMatrix);
	level->setProjMatrix(projection);
	level->Level::render();

	ball->setViewMatrix(viewMatrix);
	ball->setProjMatrix(projection);
	ball->Ball::render();
}

void GameScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	ent->Slide::update(deltaTime);

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

void GameScene::checkCollisionsAndUpdateEntitiesPositions(int deltaTime)
{	
	int time, step = 1;
	bool moveBall = true;
	for (time = step; time <= deltaTime; time += step)
	{
		vec2 newBallPosition = ball->Ball::getPosition() + (float(time/100.f) * ball->Ball::getDirection() * ball->Ball::getSpeed());

		ball->Ball::setPosition(newBallPosition);

		for (Slide* slide : level->getSlides())
		{
			// move slides
			//
		}

		contourTileList ballContourTileList = ball->listOfContourTiles();

		int upCount = 0, downCount = 0, rightCount = 0, leftCount = 0;

		for (Tile* tile : ballContourTileList.down)
			if (tile->solid)
				downCount++; 

		for (Tile* tile : ballContourTileList.up)
			if (tile->solid)
				upCount++;

		for (Tile* tile : ballContourTileList.right)
			if (tile->solid)
				rightCount++;

		for (Tile* tile : ballContourTileList.left)
			if (tile->solid)
				leftCount++;

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

			
			moveBall = false;
		}
	}


	time = time - deltaTime; // Tiempo restante no tratado en el for
	if (time > 0)
	{
		// una iteracion mas
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
	Model* m = new Model();
	m->loadFromFile("models/sphere.obj", (*defaultShaderProgram));
	models.push_back(m);
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
