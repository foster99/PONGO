#include "Slide.h"
#include "GameScene.h"
#include <glm/gtc/matrix_transform.hpp>

Slide::~Slide()
{
	this->Entity::~Entity();
}

Slide::Slide(GameScene* scene, Model* model, ShaderProgram* prog)
{
	this->Entity::Entity(scene,model,prog);
}

void Slide::init(int tileSize, int orient, vec2 pos, vec2 dir, vec2 speed, int mode)
{
	this->Slide::init();
	this->Slide::setSize(tileSize, orient);
	this->Slide::setPosition(pos);
	this->Slide::setDirection(dir);
	this->Slide::setSpeed(speed);
	this->Slide::setMode(mode);
}

void Slide::init()
{
	this->Entity::init();
	// QUITAR CUANDO SE PUEDA DISTINGUIR EL TIPO EN LA LECTURA DEL MAPA
	mode = chase;
}

void Slide::update(int deltaTime)
{
	this->Entity::update(deltaTime);

	

	if (isVertical()) {
		if (!checkTrackedPos()) {
			if (scene->ballIsOnVerticalSlideScope(this) || !trackPlayerVertical()) {
				speed.y = 0.50f;
			}
		}
	}
	else {
		if (!checkTrackedPos()) {
			if (scene->ballIsOnHorizontalSlideScope(this) || !trackPlayerHorizontal())
				speed.x = 0.50f;
		}
	}
}

void Slide::render()
{
	
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	
	float tileSize		= scene->getLevel()->getTileSize();
	float scaleFactor	= tileSize / model->getHeight();

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = translate(modelMatrix, vec3(position,0));

	if (orientation == horizontal)
		modelMatrix = glm::scale(modelMatrix, glm::vec3(4.f, 1.f, 1.f));
	else
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 4.f, 1.f));

	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -model->getCenter());

	// Compute Normal Matrix

	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

	program->use();
	program->setUniformMatrix4f("projection", projMatrix);
	program->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	program->setUniformMatrix3f("normalmatrix", normalMatrix);

	// SI FALTA ALGO PER MODIFICAR MODEL ADALT
	this->Entity::render();
}

void Slide::setSize(int ts, int orient)
{
	tileSize = ts;
	if (orient == vertical) {
		size = ivec2(1, 4);
		orientation = vertical;
		direction = vec2(0.f, 1.f);
	}
	else {
		size = ivec2(4, 1);
		orientation = horizontal;
		direction = vec2(1.f,0.f);
	}
}

void Slide::setMode(int mode)
{
	this->mode = mode;
}

void Slide::setPosition(vec2 position, int time)
{
	this->Entity::setPosition(position, time);

}

void Slide::setSpeed(vec2 speed)
{
	this->Entity::setSpeed(speed);
}

void Slide::setDirection(vec2 direction)
{
	this->Entity::setDirection(direction);
}

void Slide::goBackALittle()
{
	setPosition(position - direction * 0.1f, getPreviousTime());
}

bool Slide::checkTrackedPos()
{
	if (trackedPos.x >= 0 && mode == chase) {
		vec2 diffPos = abs(position - trackedPos);
		if (diffPos.x < distanceError && diffPos.y < distanceError) {
			return true;
		}
	}
	return false;
}

vector<vector<ivec2>> Slide::occupiedTiles()
{
	vec4 BB = Slide::getBoundingBox();

	float xmin = BB[0];
	float xmax = BB[1];
	float ymin = -BB[2];
	float ymax = -BB[3];

	float tileSize = float(scene->getLevel()->getTileSize());
	float half_tile = tileSize / 2.f;

	vector<vector<ivec2>> tiles(9, vector<ivec2>(1));

	if (isVertical())
	{
		tiles[0][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 0.f * half_tile));
		tiles[1][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 1.f * half_tile));
		tiles[2][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 2.f * half_tile));
		tiles[3][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 3.f * half_tile));
		tiles[4][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 4.f * half_tile));
		tiles[5][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 5.f * half_tile));
		tiles[6][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 6.f * half_tile));
		tiles[7][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 7.f * half_tile));
		tiles[8][0] = scene->toTileCoordsNotInverting(vec2((xmin + xmax) / 2.f, ymin + 8.f * half_tile));
	}
	else // if (isHorizontal())
	{
		tiles[0][0] = scene->toTileCoordsNotInverting(vec2(xmin + 0.f * half_tile, (ymin + ymax) / 2.f));
		tiles[1][0] = scene->toTileCoordsNotInverting(vec2(xmin + 1.f * half_tile, (ymin + ymax) / 2.f));
		tiles[2][0] = scene->toTileCoordsNotInverting(vec2(xmin + 2.f * half_tile, (ymin + ymax) / 2.f));
		tiles[3][0] = scene->toTileCoordsNotInverting(vec2(xmin + 3.f * half_tile, (ymin + ymax) / 2.f));
		tiles[4][0] = scene->toTileCoordsNotInverting(vec2(xmin + 4.f * half_tile, (ymin + ymax) / 2.f));
		tiles[5][0] = scene->toTileCoordsNotInverting(vec2(xmin + 5.f * half_tile, (ymin + ymax) / 2.f));
		tiles[6][0] = scene->toTileCoordsNotInverting(vec2(xmin + 6.f * half_tile, (ymin + ymax) / 2.f));
		tiles[7][0] = scene->toTileCoordsNotInverting(vec2(xmin + 7.f * half_tile, (ymin + ymax) / 2.f));
		tiles[8][0] = scene->toTileCoordsNotInverting(vec2(xmin + 8.f * half_tile, (ymin + ymax) / 2.f));
	}

	return tiles;
}

vec4 Slide::getBoundingBox()
{
	float lenX = float(size.x) * float(tileSize) / 2.f;
	float lenY = float(size.y) * float(tileSize) / 2.f;

	return vec4( (position.x-lenX), (position.x+lenX), (position.y+lenY), (position.y-lenY));
}

bool Slide::isVertical()
{
	return orientation == vertical;
}

bool Slide::isHorizontal()
{
	return orientation == horizontal;
}

bool Slide::trackPlayerVertical()
{
	if (this->getCurrentChunk() == scene->getPlayerChunk())
	{
		vec2 playerPosition = scene->getPlayerPos();
		vec2 playerDirection = scene->getPlayerDir();

		float dist = abs(playerPosition.x - position.x);
		dist -= tileSize;

		bool isAbove = playerPosition.y > position.y;
		bool isRight = playerPosition.x > position.x;

		float dTrack;

		dTrack = minmunTrackDistance * tileSize;

		bool trackedRight = isRight && playerDirection.x < 0 && dist < dTrack;
		bool trackedLeft = !isRight && playerDirection.x > 0 && dist < dTrack;

		if (trackedRight || trackedLeft) {
			if (mode == chase) {
				if (isAbove)
					direction.y = 1;
				else
					direction.y = -1;
				direction = normalize(direction);
				speed.y = 0.95f;
			}
			else {
				if (isAbove)
					direction.y = -1;
				else
					direction.y = 1;
				direction = normalize(direction);
				speed.y = 0.95f;
			}
			trackedPos = playerPosition;
			return true;
		}
	}
	trackedPos = vec2(-1.f, -1.f);
	return false;
}

bool Slide::trackPlayerHorizontal()
{
	if (this->getCurrentChunk() == scene->getPlayerChunk()) {
		vec2 playerPosition = scene->getPlayerPos();
		vec2 playerDirection = scene->getPlayerDir();

		float dist = abs(playerPosition.y - position.y);
		dist -= tileSize;

		bool isAbove = playerPosition.y > position.y;
		bool isRight = playerPosition.x > position.x;

		float dTrack; 
		
		dTrack = minmunTrackDistance * tileSize;

		bool trackedUp = isAbove && playerDirection.y < 0 && dist < dTrack;
		bool trackedDown = !isAbove && playerDirection.y > 0 && dist < dTrack;

		if (trackedUp || trackedDown) {
			if (mode == chase) {
				if (isRight)
					direction.x = 1;
				else
					direction.x = -1;
				direction = normalize(direction);
				speed.x = 0.95f;
			}
			else {
				if (isRight)
					direction.x = -1;
				else
					direction.x = 1;
				direction = normalize(direction);
				speed.x = 0.95f;
			}

			trackedPos = playerPosition;
			return true;
		}
	}
	trackedPos = vec2(-1.f, -1.f);
	return false;
}
