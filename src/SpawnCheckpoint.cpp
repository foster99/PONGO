#include "SpawnCheckpoint.h"
#include "GameScene.h"
#include <glm/gtc/matrix_transform.hpp>

SpawnCheckpoint::~SpawnCheckpoint()
{
	this->Entity::~Entity();

}

SpawnCheckpoint::SpawnCheckpoint(GameScene* scene, Model* model, ShaderProgram* prog)
{
	this->Entity::Entity(scene, model, prog);
}

void SpawnCheckpoint::init()
{
	this->Entity::init();
	renderable = true;
	triggered = false;
}

void SpawnCheckpoint::update(int deltaTime)
{

}

void SpawnCheckpoint::render()
{
	if (renderable) {
		glm::mat4 modelMatrix;
		glm::mat3 normalMatrix;

		float tileSize = scene->getLevel()->getTileSize();
		float scaleFactor = tileSize / model->getHeight();

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = translate(modelMatrix, vec3(position, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
		modelMatrix = glm::translate(modelMatrix, -model->getCenter());

		normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

		program->use();
		program->setUniformMatrix4f("projection", projMatrix);
		program->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
		program->setUniformMatrix3f("normalmatrix", normalMatrix);

		// SI FALTA ALGO PER MODIFICAR MODEL ADALT
		this->Entity::render();
	}
}

void SpawnCheckpoint::setPosition(vec2 position, int time)
{
	this->Entity::setPosition(position, time);
}

void SpawnCheckpoint::setSpeed(vec2 speed)
{
	this->Entity::setSpeed(speed);
}

void SpawnCheckpoint::setDirection(vec2 direction)
{
	this->Entity::setDirection(direction);
}

vector<vector<ivec2>> SpawnCheckpoint::occupiedTiles()
{
	vec4 BB = SpawnCheckpoint::getBoundingBox();

	float xmin = BB[0];
	float xmax = BB[1];
	float ymin = BB[2];
	float ymax = BB[3];

	vector<vector<ivec2>> tiles(2, vector<ivec2>(2));
	tiles[0][0] = scene->toTileCoords(vec2(xmin, ymax));
	tiles[0][1] = scene->toTileCoords(vec2(xmax, ymax));
	tiles[1][0] = scene->toTileCoords(vec2(xmin, ymin));
	tiles[1][1] = scene->toTileCoords(vec2(xmax, ymin));

	return tiles;
}

vec4 SpawnCheckpoint::getBoundingBox()
{
	float displacement = float(scene->getLevel()->getTileSize()) / 2.f;

	vec4 BB(position.x, position.x, position.y, position.y);

	BB[0] -= displacement;
	BB[1] += displacement;
	BB[2] -= displacement;
	BB[3] += displacement;

	return BB;
}

void SpawnCheckpoint::collided()
{
	if (!triggered) {
		scene->getLevel()->addSpawnPoint(position);
		triggered = true;
	}
}
