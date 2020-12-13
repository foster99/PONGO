#include "GameScene.h"
#include "Game.h"

void GameScene::init()
{
	this->Scene::init();

	levelID = 1;
	restartLevel();

	speedDivisor = float(level->getTileSize());
	
	loadCountDownModels();
}

void GameScene::render()
{
	renderAxis();

	if (countdown > 0)
		renderCountDown();

	level->Level::render();

	if (!dead)
		ball->Ball::render();
}

void GameScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	updateCurrentChunk();
	transitionControlUpdate();
	updateViewMatrix();

	level->update(deltaTime);
	ball->Ball::update(deltaTime);


	if (onCountDown(deltaTime)) return;

	if (dead)
	{
		if (deadtime > 0)
		{
			deadtime -= deltaTime;
			return;
		}

		ball->locateInSpawnPoint();
		dead = false;
	}

	if (win)
	{
		Game::instance().stopLevelSong();
		if (wintime > 0)
		{
			wintime -= deltaTime;
			return;
		}

		loadNextLevel();
		return;
	}

	// Rope blocks
	ball->restartDirectionBlocks();

	if (ballIsOnHorizontalRope())
		blockBallsVerticalDirection();

	if (ballIsOnVerticalRope())	
		blockBallsHorizontalDirection();

	// Collisions
	checkCollisionsAndUpdateEntitiesPositions(deltaTime);
	level->checkIfBallCollidedWithACheckpoint();
	clearPositionHistories();
}

void GameScene::restartLevel()
{
	initBall();

	level = new Level(this, levelID);
	level->setViewMatrix(view);
	level->setProjMatrix(projection);

	dead = false;
	win = false;
	currentChunk = 0;
	transitionR = false;
	transitionL = false;
	transitionU = false;
	transitionD = false;
	countdown = 3000;
	countdownStarted = false;
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

	if (!countdownStarted)
	{
		countdownStarted = true;
		Game::instance().stopBackgroundSong();
		Game::instance().playCountdownSound();
	}

	countdown -= deltaTime;
	
	if (countdown < 200)
		Game::instance().playLevelSong();

	
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
	float zooming;

	if (countdown > 2000)
	{
		model = model_3;
		zooming = smoothstep(3000.f, 2000.f, float(countdown));
	}
	else if (countdown > 1000)
	{
		model = model_2;
		zooming = smoothstep(2000.f, 1000.f, float(countdown));
	}
	else
	{
		model = model_1;
		zooming = smoothstep(1000.f, 0.f, float(countdown));
	}

	float scale_factor = zooming * 10.5f * float(getLevel()->getTileSize()) / model->getHeight();
	
	vec3 position	= getCameraChunkPosition() * vec3(1.f, 1.f, 0.33f);
	mat4 myView		= lookAtCurrentChunk();

	// Compute ModelMatrix
	mat4 modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, position);
	modelMatrix = scale(modelMatrix, vec3(scale_factor)/ 2.f);
	modelMatrix = translate(modelMatrix, -model->getCenter());

	// Compute NormalMatrix
	mat3 normalMatrix = transpose(inverse(mat3(myView * modelMatrix)));

	// Set uniforms
	numbersShader->use();
	numbersShader->setUniform1b("bLighting", true);
	numbersShader->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	numbersShader->setUniformMatrix4f("projection", projection);
	numbersShader->setUniformMatrix4f("modelview", myView * modelMatrix);
	numbersShader->setUniformMatrix3f("normalmatrix", normalMatrix);

	model->render(*(numbersShader));
}

mat4 GameScene::countDownViewMatrix()
{
	// Compute chunk displacement
	vec2 distanceToChunkCentre;
	distanceToChunkCentre = float(level->getTileSize()) * (vec2(level->getChunkSize()) - vec2(1.f)) / 2.f;
	distanceToChunkCentre = vec2(1, -1) * distanceToChunkCentre;

	// Compute target Chunks
	ivec2 mapSizeInChunks = level->getMapSizeInChunks();
	int startChunk		= currentChunk;
	int topCenterChunk	= mapSizeInChunks.x / 2;
	int botCenterChunk	= topCenterChunk + (mapSizeInChunks.y - 1) * mapSizeInChunks.x;

	// Compute Target Chunk Centres
	vec2 startChunkCentre		= level->getFirstTileOfChunk(startChunk)->coords + distanceToChunkCentre;
	vec2 topCenterChunkCentre	= level->getFirstTileOfChunk(topCenterChunk)->coords + distanceToChunkCentre;
	vec2 botCenterChunkCentre	= level->getFirstTileOfChunk(botCenterChunk)->coords + distanceToChunkCentre;

	float zDisplacement1 = float(level->getTileSize()) * (float(glm::min(level->getMapSizeInChunks().x * level->getChunkSize().x, level->getMapSizeInChunks().y * level->getChunkSize().y)) + 1.f) / 2.f;

	if (countdown > 2000)
	{
		float extraDisp = 600.f * (1.f - smoothstep(3000.f, 2000.f, float(countdown)));
		return lookAt(vec3(botCenterChunkCentre, zDisplacement1 + extraDisp), vec3(botCenterChunkCentre, 0.f), vec3(0.f, 1.f, 0.f));
	}

	if (countdown > 500)
	{
		// Compute Distances between Chunks
		vec2 direction_1	= normalize(topCenterChunkCentre - botCenterChunkCentre);
		float distance_1	= length(topCenterChunkCentre - botCenterChunkCentre);
		float pasito_1		= smoothstep(2000.f, 500.f, float(countdown));

		vec2 myCoords = botCenterChunkCentre + (direction_1 * distance_1 * pasito_1);

		return lookAt(vec3(myCoords, zDisplacement1), vec3(myCoords, 0.f), vec3(0.f, 1.f, 0.f));
	}


	vec2 direction_2	= normalize(startChunkCentre - topCenterChunkCentre);
	float distance_2	= length(startChunkCentre - topCenterChunkCentre);
	float pasito_2		= smoothstep(500.f, 0.f, float(countdown));

	vec2 myCoords = topCenterChunkCentre + (direction_2 * distance_2 * pasito_2);
	
	float zDisplacement2 = float(level->getTileSize()) * (float(glm::max(level->getChunkSize().x, level->getChunkSize().y)) + 1.f) / 2.f;
	float extraDisp = (zDisplacement1 - zDisplacement2) * (1.f - pasito_2);


	return lookAt(vec3(myCoords, zDisplacement2 + extraDisp), vec3(myCoords, 0.f), vec3(0.f, 1.f, 0.f));
}

void GameScene::clearPositionHistories()
{
	ball->clearHistory();

	for (Slide* slide : level->getSlides())
		slide->clearHistory();
}

void GameScene::killBall()
{
	if (Game::instance().isInGodMode()) return;

	dead	 = true;
	deadtime = 1000.f;
	Game::instance().playDeathSound();
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

bool GameScene::checkCollision_Ball_World(int tick, int deltaTime)
{
	vec2 displacement    = float(deltaTime) * ball->Ball::getSpeed() / (float(nTicks) * speedDivisor);
	vec2 newBallPosition = ball->Ball::getPosition() + displacement * ball->getDirection();

	ball->Ball::setPosition(newBallPosition, tick);

	contourTileList ballContourTileList = ball->listOfContourTiles();

	int upCount = 0, downCount = 0, rightCount = 0, leftCount = 0;

	for (Tile* tile : ballContourTileList.down)
	{
		if (tile->solid)					downCount++;
		if (tile->type == Tile::winTile)	winLevel();
	}
	for (Tile* tile : ballContourTileList.up)
	{
		if (tile->solid)					upCount++;
		if (tile->type == Tile::winTile)	winLevel();
	}
	for (Tile* tile : ballContourTileList.right)
	{
		if (tile->solid)					rightCount++;
		if (tile->type == Tile::winTile)	winLevel();
	}
	for (Tile* tile : ballContourTileList.left)
	{
		if (tile->solid)					leftCount++;
		if (tile->type == Tile::winTile)	winLevel();
	}

	
	if (ball->isOnDeathTiles()) killBall();


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

bool GameScene::checkCollision_Slide_World(Slide* slide, int tick, int deltaTime)
{
	vec2 oldSlidePosition = slide->Slide::getPosition();
	vec2 newSlidePosition = oldSlidePosition + (float(deltaTime) * slide->Slide::getDirection() * slide->Slide::getSpeed() / (float(nTicks) * speedDivisor));

	slide->setPosition(newSlidePosition, tick);

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

	for (int tick = 1; tick <= nTicks; tick++)
	{
		if (!ballCollidedWithWorld)
			ballCollidedWithWorld = checkCollision_Ball_World(tick, deltaTime);

		bool slideCollidedWorld;
		for (Slide* slide : level->getSlides())
		{
			slideCollidedWorld = checkCollision_Slide_World(slide, tick, deltaTime);
		}
	}

	if (!Game::instance().isInGodMode())
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

	Game::instance().playHitmarkerSound();

	while (slide->getPreviousTime() > ball->getPreviousTime() && slide->rollbackPosition());
	while (ball->getPreviousTime() > slide->getPreviousTime() && ball->rollbackPosition());

	while (slide->getPreviousTick() > ball->getPreviousTick() && slide->rollbackPosition());
	while (ball->getPreviousTick() > slide->getPreviousTick() && ball->rollbackPosition());

	while (ballAndSlideAreColliding(slide) && ball->rollbackPosition() && slide->rollbackPosition());


	if (ballIsOnVerticalSlideScope(slide))
	{
		ball->invertDirectionY();

		if (slide->isVertical())
		{
			// Colision de Canto

			if ((ball->getDirection().y < 0) ^ (slide->getDirection().y < 0))
				ball->invertDirectionY();

			slide->goBackALittle();
			ball->displacePosition(ball->getSpeed() * vec2((ball->getDirection()).x, 0.f));
		}
	}
	if (ballIsOnHorizontalSlideScope(slide))
	{
		ball->invertDirectionX();

		if (slide->isHorizontal())
		{
			// Colision de Canto

			if ((ball->getDirection().x < 0) ^ (slide->getDirection().x < 0))
				ball->invertDirectionX();

			slide->goBackALittle();
			ball->displacePosition(ball->getSpeed() * vec2(0.f, (ball->getDirection()).y));
		}
	}
}

bool GameScene::ballIsOnHorizontalRope()
{
	Tile* tile = level->getTile(toTileCoords(ball->getPosition()));
	return (tile->type == Tile::ropeH);
}

bool GameScene::ballIsOnVerticalRope()
{
	Tile* tile = level->getTile(toTileCoords(ball->getPosition()));
	return (tile->type == Tile::ropeV);
}

void GameScene::blockBallsHorizontalDirection()
{
	ball->blockHorizontalDirection();
}

void GameScene::blockBallsVerticalDirection()
{
	ball->blockVerticalDirection();
}

void GameScene::updateViewMatrix()
{
	if (win && wintime > 0)			view = winAnimationViewMatrix();
	else if (countdown > 0)			view = countDownViewMatrix();
	else if (cam->isFree())			view = cam->getViewMatrix();
	else if (weAreInTransition())	view = transitionMatrix();
	else							view = lookAtCurrentChunk();
}

bool GameScene::weAreInTransition()
{
	return transitionR || transitionL || transitionU || transitionD;
}

void GameScene::transitionControlUpdate()
{
	ivec2 tileCoords = toTileCoords(ball->getPosition());
	int i = tileCoords.y;
	int j = tileCoords.x;


	ivec2 mapSize = level->getMapSizeInChunks();
	bool inMapLimitR = (currentChunk % mapSize.x == mapSize.x - 1);
	bool inMapLimitL = (currentChunk % mapSize.x == 0);
	bool inMapLimitD = (currentChunk / mapSize.x == mapSize.y - 1);
	bool inMapLimitU = (currentChunk / mapSize.x == 0);

	ivec2 chunkSize = level->getChunkSize();
	int i_chunk = i % chunkSize.y;
	int j_chunk = j % chunkSize.x;

	int limitR = chunkSize.x - 1 - offsetX;
	int limitL = offsetX;
	int limitD = chunkSize.y - 1 - offsetY;
	int limitU = offsetY;

	transitionR = !inMapLimitR && j_chunk >= limitR;
	transitionL = !inMapLimitL && j_chunk <= limitL;
	if (transitionR && transitionL)
		transitionD = transitionD;
	transitionD = !inMapLimitD && i_chunk >= limitD;
	transitionU = !inMapLimitU && i_chunk <= limitU;
}

mat4 GameScene::transitionMatrix()
{
	float tileSize		= float(level->getTileSize());
	float zDisplacement	= tileSize * (float(glm::max(level->getChunkSize().x, level->getChunkSize().y)) + 1.f) / 2.f;
	vec2 chunkTopLeft = level->getFirstTileOfChunk(currentChunk)->coords - vec2(tileSize / 2.f, - tileSize / 2.f);
	vec2 chunkLength = tileSize * vec2(level->getChunkSize());

	float chunkU = chunkTopLeft.y;
	float chunkL = chunkTopLeft.x;
	float chunkD = chunkU - chunkLength.y;
	float chunkR = chunkL + chunkLength.x;

	vec3 currentChunkCenterCoords = getCameraChunkPosition();
	vec2 ballPosition = ball->getPosition();

	float start, end;
	vec2 transitionCoords(currentChunkCenterCoords);

    if (transitionR)
	{
		start	= chunkR - float(offsetX) * tileSize;
		end		= chunkR;
		transitionCoords.x += smoothstep(start, end + (end - start), ballPosition.x) * chunkLength.x;
	}
	
	if (transitionL)
	{
		start	= chunkL;
		end		= chunkL + float(offsetX) * tileSize;
		transitionCoords.x -= (1.f - smoothstep(start - (end - start), end , ballPosition.x)) * chunkLength.x;
	}

	if (transitionD)
	{
		start	= chunkD + float(offsetY) * tileSize;
		end		= chunkD;
		transitionCoords.y -= smoothstep(start, end + (end - start), ballPosition.y) * chunkLength.y;
	}

	if (transitionU)
	{
		start	= chunkU;
		end		= chunkU - float(offsetY) * tileSize;;
		transitionCoords.y += (1.f - smoothstep(start - (end - start), end, ballPosition.y)) * chunkLength.y;
	}

	return lookAt(vec3(transitionCoords, zDisplacement), vec3(transitionCoords, 0.f), vec3(0.f, 1.f, 0.f));
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
	vec3 currentChunkCenterCoords = getCameraChunkPosition();

	return lookAt(currentChunkCenterCoords, vec3(currentChunkCenterCoords.x, currentChunkCenterCoords.y, 0.f), vec3(0.f, 1.f, 0.f));
}

void GameScene::winLevel()
{
	Game::instance().stopBackgroundSong();
	teleportSound = false;
	win = true;
	wintime = 3000;
}

mat4 GameScene::winAnimationViewMatrix()
{
	// Compute Target Chunk Centres
	vec3 chunkCentre = getCameraChunkPosition();
	vec3 ballCentre = vec3(ball->getPosition(), chunkCentre.z / 2.f);

	if (wintime < 2500 && !teleportSound)
	{
		Game::instance().playWinLevelSound();
		teleportSound = true;
	}

	if (wintime > 2000)
	{
		// Compute Distances between Chunks
		vec3 direction_1 = normalize(ballCentre - chunkCentre);
		float distance_1 = length(ballCentre - chunkCentre);
		float pasito_1   = smoothstep(3000.f, 2000.f, float(wintime));

		vec3 myCoords = chunkCentre + (direction_1 * distance_1 * pasito_1);

		return lookAt(myCoords, vec3(vec2(myCoords), 0.f), vec3(0.f, 1.f, 0.f));
	}


	float extraDisp = 400.f * smoothstep(2000.f, 0.f, float(wintime));

	vec3 OBS = ballCentre - vec3(0.f, 0.f, extraDisp);
	vec3 VRP = OBS - vec3(0.f, 0.f, 1.f);
	vec3 UP  = vec3(0.f, 1.f, 0.f);

	return lookAt(OBS, VRP, UP);
}

void GameScene::loadNextLevel()
{
	if (levelID == lastLevel)
	{
		winGame();
		return;
	}

	levelID++;
	restartLevel();
}

void GameScene::winGame()
{
	// End game
	// Show thanks
	// Go to Start Menu
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

	if (ballIsOnHorizontalRope())
		ball->invertDirectionX();
	
	ball->invertDirectionY();

	Game::instance().playGotaSound();
}

int GameScene::getLevelID()
{
	return levelID;
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

// PURE TESTING REMOVE LATER
void GameScene::changeSpawnPoint()
{
	level->changeSpawnPoint();
}

vec4 GameScene::getPlayerBBox()
{
	return ball->Ball::getBoundingBox();
}

int GameScene::getPlayerChunk()
{
	return ball->getCurrentChunk();
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
