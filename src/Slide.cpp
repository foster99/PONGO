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

void Slide::init()
{
	this->Entity::init();

	// SI FALTA ALGO ABAIX
	tracked = false;
	initialized = false;
	speed = 1.0f;
	position = ogPos = vec2(32, -48);
}

void Slide::update(int deltaTime)
{
	// UPDATES EN UN FUTUR QUANT FOWOSTER DIGUI NOMES HAN DE CANVIAR DIRECTION I SPEED
	// SET POSTITION CANVIARA RESPECTE AQUESTES DADES
	if (orientation == vertical) {
		if (!trackPlayerVertical())
			updateVertical(deltaTime);
	}
	else {
		if (!trackPlayerHorizontal())
			updateHorizontal(deltaTime);
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
		modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 1, 1));
	else
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 4, 1));

	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -model->getCenter());

	// Compute Normal Matrix

	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

	program->use();
	program->setUniformMatrix4f("projection", projection);
	program->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	program->setUniformMatrix3f("normalmatrix", normalMatrix);

	// SI FALTA ALGO PER MODIFICAR MODEL ADALT
	this->Entity::render();
}

void Slide::updateVertical(int deltaTime)
{

	return;
	float len  = size.y * tileSize;
	float limY = limits.y * tileSize;

	position.y += speed * direction.y;

	if (position.y < (ogPos.y - size.y - limY ) || (position.y + len) > (ogPos.y + len + limY)) {
		position.y -= speed * direction.y;
		direction.y *= -1;
	}
}

void Slide::updateHorizontal(int deltaTime)
{
	return;
	float len  = size.x * tileSize;
	float limX = limits.x * tileSize;

	position.x += speed * direction.x;

	if (position.x < (ogPos.x - size.x - limX) || (position.x + len) >(ogPos.x + len + limX)) {
		position.x -= speed * direction.x;
		direction.x *= -1;
	}
}

void Slide::setSize(int ts, int orient)
{
	tileSize = ts;
	if (orient == vertical) {
		size = ivec2(1, 4);
		orientation = vertical;
		direction = vec2(0, 1);
	}
	else {
		size = ivec2(4, 1);
		orientation = horizontal;
		direction = vec2(1,0);
	}
}

void Slide::setLimits(int head, int tail)
{
	limits = ivec2(head, tail);
}

void Slide::setPosition(vec2 position)
{
	this->Entity::setPosition(position);
	
	if (!initialized) {
		ogPos = position;
		initialized = true;
	}
}

void Slide::setSpeed(vec2 speed)
{
	this->Entity::setSpeed(speed);
}

void Slide::setDirection(vec2 direction)
{
	this->Entity::setDirection(direction);
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

	vector<vector<ivec2>> tiles;

	if (isVertical())
	{
		tiles.resize(9, vector<ivec2>(1));
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
		tiles.resize(9, vector<ivec2>(1));
		tiles[0][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 0.f * half_tile));
		tiles[1][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 1.f * half_tile));
		tiles[2][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 2.f * half_tile));
		tiles[3][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 3.f * half_tile));
		tiles[4][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 4.f * half_tile));
		tiles[5][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 5.f * half_tile));
		tiles[6][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 6.f * half_tile));
		tiles[7][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 7.f * half_tile));
		tiles[8][0] = scene->toTileCoordsNotInverting(vec2((ymin + ymax) / 2.f, xmin + 8.f * half_tile));
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
	return false;
}

bool Slide::trackPlayerHorizontal()
{
	return false;
}
