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

	if (cam->isFree())	view = cam->getViewMatrix();
	else				view = lookAtCurrentChunk();

	ball->Ball::update(deltaTime);
	level->update(deltaTime);

	ball->Ball::update(deltaTime);
	updateCurrentChunk();
	checkCollisionsAndUpdateEntitiesPositions(deltaTime);
	clearPositionHistories();
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

void GameScene::clearPositionHistories()
{
	ball->clearHistory();

	for (Slide* slide : level->getSlides())
		slide->clearHistory();
}

bool GameScene::ballIsOnHorizontalSlideScope(Slide* slide)
{
	vec4 ball_BB	= ball->Ball::getBoundingBox();
	vec4 slide_BB	= slide->Slide::getBoundingBox();

	float ymin_1 = -ball_BB[3];
	float ymax_1 = -ball_BB[2];

	float ymin_2 = -slide_BB[2];
	float ymax_2 = -slide_BB[3];

	return (ymin_1 <= ymax_2 && ymax_1 >= ymin_2);
}

bool GameScene::ballIsOnVerticalSlideScope(Slide* slide)
{
	vec4 ball_BB = ball->Ball::getBoundingBox();
	vec4 slide_BB = slide->Slide::getBoundingBox();

	float xmin_1 = ball_BB[0];
	float xmax_1 = ball_BB[1];

	float xmin_2 = slide_BB[0];
	float xmax_2 = slide_BB[1];

	return (xmin_1 <= xmax_2 && xmax_1 >= xmin_2);
}

bool GameScene::collidingBoundingBoxes(vec4 BB1, vec4 BB2)
{
	float xmin_1 =  BB1[0];
	float xmax_1 =  BB1[1];
	float ymin_1 = -BB1[3];
	float ymax_1 = -BB1[2];

	float xmin_2 =  BB2[0];
	float xmax_2 =  BB2[1];
	float ymin_2 = -BB2[2];
	float ymax_2 = -BB2[3];

	return	(xmin_1 <= xmax_2 && xmax_1 >= xmin_2) &&
			(ymin_1 <= ymax_2 && ymax_1 >= ymin_2);
}

bool GameScene::checkCollision_Ball_World(int time)
{
	vec2 displacement = (float(time / 100.f) * ball->Ball::getSpeed());
	vec2 newBallPosition = ball->Ball::getPosition() + displacement * ball->getDirection();

	ball->Ball::setPosition(newBallPosition, time);

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
		ball->rollbackPosition();

		if (downCount > 1 && downCount > leftCount && downCount > rightCount)
		{
			ball->invertDirectionY();
			ball->displacePosition(vec2(0.f, displacement.y));
		}
		else if (upCount > 1 && upCount > leftCount && upCount > rightCount)
		{
			ball->invertDirectionY();
			ball->displacePosition(vec2(0.f, -displacement.y));
		}

		if (leftCount > 1 && leftCount > downCount && leftCount > upCount)
		{
			ball->invertDirectionX();
			ball->displacePosition(vec2(displacement.x, 0.f));
		}
		else if (rightCount > 1 && rightCount > downCount && rightCount > upCount)
		{
			ball->invertDirectionX();
			ball->displacePosition(vec2(-displacement.x, 0.f));
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

	slide->setPosition(newSlidePosition, time);

	vector<vector<ivec2>> tiles = slide->Slide::occupiedTiles();
	
	for (auto row : tiles)
	{
		if (level->getTile(row[0])->solid)
		{
			slide->rollbackPosition();

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
		if (!ballCollidedWithWorld)
			ballCollidedWithWorld = checkCollision_Ball_World(time);

		bool slideCollidedWorld;
		for (Slide* slide : level->getSlides())
		{
			slideCollidedWorld = checkCollision_Slide_World(slide, time);
		}
	}

	// aqui

	Slide* slide = level->whichSlideIsCollidingWithTheBall();

	if (slide == nullptr) return;

	while (slide->getPreviousTime() > ball->getPreviousTime() && slide->rollbackPosition());

	while (ball->getPreviousTime() > slide->getPreviousTime() && ball->rollbackPosition());

	while (ballAndSlideAreColliding(slide) && ball->rollbackPosition() && slide->rollbackPosition());


	if (ballIsOnVerticalSlideScope(slide))
	{
		ball->invertDirectionY();

		if (slide->isVertical())
		{
			slide->goBackALittle();
			ball->displacePosition(ball->getSpeed() * vec2((ball->getDirection()).x, 0.f));
		}
	}
	if (ballIsOnHorizontalSlideScope(slide))
	{
		ball->invertDirectionX();

		if (slide->isHorizontal())
		{
			slide->goBackALittle();
			ball->displacePosition(ball->getSpeed() * vec2(0.f, (ball->getDirection()).y));
		}
	}
}



bool GameScene::ballAndSlideAreColliding(Slide* slide)
{
	return collidingBoundingBoxes(ball->Ball::getBoundingBox(), slide->Slide::getBoundingBox());
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
	ball->spawnParticles();
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

vec3 GameScene::getCameraChunkPosition() {

	vec2 distanceToChunkCentre, chunkCentre;

	// TileSize * (chunkSize - 1) / 2   ---->  El 1 esta para desplazar medio cubo extra
	distanceToChunkCentre = float(level->getTileSize()) * (vec2(level->getChunkSize()) - vec2(1.f)) / 2.f;
	distanceToChunkCentre = vec2(1, -1) * distanceToChunkCentre;
	chunkCentre = level->getFirstTileOfChunk(currentChunk)->coords + distanceToChunkCentre;

	float zDisplacement = float(level->getTileSize()) * (float(glm::max(level->getChunkSize().x, level->getChunkSize().y)) + 1.f) / 2.f;

	return vec3(chunkCentre, zDisplacement);
}

vec3 GameScene::getCameraPosition()
{
	if(cam->isFree())
		return cam->getCameraPosition();
	
	return getCameraChunkPosition();
}

mat4 GameScene::getViewMatrix()
{
	return view;
}

mat4 GameScene::getProjMatrix()
{
	return projection;
}
