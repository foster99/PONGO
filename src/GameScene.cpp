#include "GameScene.h"

void GameScene::init()
{
	this->Scene::init();

	levelID = 1;
	restartLevel(levelID);

	loadCountDownModels();
}

void GameScene::render()
{
	renderAxis();

	if (countdown > 0) renderCountDown();


	level->Level::render();
	ball->Ball::render();
}

void GameScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	if (cam->isFree())	view = cam->getViewMatrix();
	else				view = lookAtCurrentChunk();

	level->update(deltaTime);
	ball->Ball::update(deltaTime);
	updateCurrentChunk();

	if (onCountDown(deltaTime)) return;

	if (dead)
	{
		ball->locateInSpawnPoint();
		dead = false;
	}


	checkCollisionsAndUpdateEntitiesPositions(deltaTime);
	//clearPositionHistories();
}

void GameScene::restartLevel(int levelID)
{
	initBall();

	level = new Level(this, levelID);
	level->setViewMatrix(view);
	level->setProjMatrix(projection);

	dead = false;
	currentChunk = 0;
	countdown = 3000;

	// Restart Song
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

bool GameScene::onCountDown(int deltaTime)
{
	if (countdown <= 0) return false;

	countdown -= deltaTime;

	return true;
}

void GameScene::loadCountDownModels()
{
	// Load Shader
	numbersShader = new ShaderProgram();
	loadShaders("numbersShader", numbersShader);

	// Load Models
	model_1 = new Model();
	model_1->loadFromFile("models/one.obj", (*ballShader));

	model_2 = new Model();
	model_2->loadFromFile("models/two.obj", (*ballShader));

	model_3 = new Model();
	model_3->loadFromFile("models/three.obj", (*ballShader));
}

void GameScene::renderCountDown()
{
	Model* model;

	     if (countdown > 2000) model = model_3;
	else if (countdown > 1000) model = model_2;
	else                       model = model_1;

	float scale_factor = 5.f * float(getLevel()->getTileSize()) / model->getHeight();
	
	vec3 position = getCameraChunkPosition() * vec3(1.f, 1.f, 0.33f);

	// Compute ModelMatrix
	mat4 modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix = scale(modelMatrix, vec3(scale_factor, scale_factor, scale_factor / 2.f));
	modelMatrix = translate(modelMatrix, -model->getCenter());

	// Compute NormalMatrix
	mat3 normalMatrix = transpose(inverse(mat3(view * modelMatrix)));

	// Set uniforms
	numbersShader->use();
	numbersShader->setUniform1b("bLighting", true);
	numbersShader->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	numbersShader->setUniformMatrix4f("projection", projection);
	numbersShader->setUniformMatrix4f("modelview", view * modelMatrix);
	numbersShader->setUniformMatrix3f("normalmatrix", normalMatrix);

	model->render(*(numbersShader));

}

void GameScene::clearPositionHistories()
{
	ball->clearHistory();

	for (Slide* slide : level->getSlides())
		slide->clearHistory();
}

void GameScene::killBall()
{
	dead = true;
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

	for (Tile* tile : ballContourTileList.down)
	{
		if (tile->deadly) killBall();
		if (tile->solid) downCount++;
	}
	for (Tile* tile : ballContourTileList.up)
	{
		if (tile->deadly) killBall();
		if (tile->solid) upCount++;
	}
	for (Tile* tile : ballContourTileList.right)
	{
		if (tile->deadly) killBall();
		if (tile->solid) rightCount++;
	}
	for (Tile* tile : ballContourTileList.left)
	{
		if (tile->deadly) killBall();
		if (tile->solid) leftCount++;
	}

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

	checkCollision_Ball_Slide();
}

bool GameScene::ballAndSlideAreColliding(Slide* slide)
{
	return collidingBoundingBoxes(ball->Ball::getBoundingBox(), slide->Slide::getBoundingBox());
}

void GameScene::checkCollision_Ball_Slide()
{
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

int GameScene::getChunkOfCoords(vec2 coords)
{
	return level->getTile(toTileCoords(coords))->chunk;
}

void GameScene::updateCurrentChunk()
{
	currentChunk = getChunkOfCoords(ball->getPosition());
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

void GameScene::setSpawnPoint(vec2 coords)
{
	ball->setSpawnPoint(coords);
}

void GameScene::locateBallInSpawnPoint()
{
	ball->locateInSpawnPoint();
}

vec4 GameScene::getPlayerBBox()
{
	return ball->Ball::getBoundingBox();
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
