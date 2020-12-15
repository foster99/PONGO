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

	particles = new ParticleSystem();
	particles->init(glm::vec2(2.f, 2.f), (*program), "images/particle.png", 0.f);

	currentTime = 0;
}

void SpawnCheckpoint::update(int deltaTime)
{
	currentTime += deltaTime;
	particles->update(deltaTime / 1000.f);
}

void SpawnCheckpoint::render()
{
	if (renderable) {
		glm::mat4 modelMatrix;
		glm::mat3 normalMatrix;

		Model* currModel = model;
		if (triggered)
			currModel = secondModel;

		float tileSize = scene->getLevel()->getTileSize();
		vec3 displacement = vec3(0.f, tileSize*0.45f, 0.f);
		float scaleFactor = 1.5f*tileSize / currModel->getHeight();

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = translate(modelMatrix, vec3(position + tileSize * 0.05f, 0.f) + displacement);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
		modelMatrix = glm::translate(modelMatrix, -currModel->getCenter());

		normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

		program->use();
		program->setUniform1b("bLighting", true);
		program->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		program->setUniformMatrix4f("projection", projMatrix);
		program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
		program->setUniformMatrix3f("normalmatrix", normalMatrix);

		// SI FALTA ALGO PER MODIFICAR MODEL ADALT
		currModel->render((*program));
		//this->Entity::render();

		program->setUniform1b("bLighting", false);
		modelMatrix = glm::mat4(1.0f);
		program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
		normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
		program->setUniformMatrix3f("normalmatrix", normalMatrix);
		particles->render(scene->getCameraPosition());
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

void SpawnCheckpoint::setSecondModel(Model* mod)
{
	secondModel = mod;
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
	spawnParticles();
}

void SpawnCheckpoint::spawnParticles()
{
	int nParticlesToSpawn = 18;
	ParticleSystem::Particle particle;
	float angleStep = 360.f / (float)nParticlesToSpawn;
	float step = 15.f;

	particle.lifetime = 1.4f;
	for (int i = 0; i < nParticlesToSpawn; i++)
	{
		float currAngle = (angleStep * i) * (PI / 180.f);
		particle.position = vec3(position, 0) + vec3(step,1,step) * vec3(cos(currAngle), -1.75f, sin(currAngle));

		particle.speed = -step * vec3(cos(currAngle), 1, sin(currAngle));
		particles->addParticle(particle);
	}
}

void SpawnCheckpoint::noRender()
{
	renderable = false;
}
